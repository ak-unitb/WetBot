#!/bin/ash
####
##
## !!-> assuming there is a sd-card!
## @see: http://arduino.cc/en/Tutorial/ExtendingYunDiskSpace
## 
## http://wiki.openwrt.org/doc/howto/lamp
## http://wiki.openwrt.org/doc/howto/php
##
## @TODO: check for existing sd-card "if [[-d /mnt/sda1]]"
## @TODO: check for correct overlay on /mnt/sda1 ... (and study before ;) )
## @TODO: (URGENT!) delete default mysql root user without password, or change them to the same password!
##             - mysql> SET PASSWORD FOR 'root'@'Arduino' = PASSWORD('<THE_CHOOSEN_PASSWORD>');
##             - mysql> SET PASSWORD FOR 'root'@'127.0.0.1' = PASSWORD('<THE_CHOOSEN_PASSWORD>');
## @TODO: _just_ for development purposes create a mysql user 'root'@'%' for easier acces to DB, but print
##        a warning and provide an option to skip (default is skipping!)
##             - CREATE USER 'root'@'%' IDENTIFIED BY '<THE_CHOOSEN_PASSWORD>';
##	           - GRANT ALL PRIVILEGES ON *.* TO 'root'@'%';
## @TODO: Write example index.php to /mnt/sda1/arduino/www
## @TODO: ask for required Timezone and modify /etc/php.ini appropriate
##
####


### install php5

opkg update
opkg install zoneinfo-core zoneinfo-europe

opkg update
opkg install php5 php5-cgi php5-cli

uci add_list uhttpd.main.interpreter=".php=/usr/bin/php-cgi"
uci set uhttpd.main.index_page="index.html index.php"
uci commit uhttpd

sed -i 's,doc_root.*,doc_root = "",g' /etc/php.ini
sed -i 's,;short_open_tag = Off,short_open_tag = On,g' /etc/php.ini



### install MySQL

mysqlBaseDir=/mnt/sda1/mysql
mysqlDataDir=$mysqlBaseDir/data
mysqlTmpDir=$mysqlBaseDir/tmp

mkdir $mysqlBaseDir
mkdir $mysqlDataDir
mkdir $mysqlTmpDir

opkg update
opkg install libpthread libncurses libreadline mysql-server

sed -i 's,^datadir.*,datadir          = "'$mysqlDataDir'",g' /etc/my.cnf
sed -i 's,^tmpdir.*,tmpdir          = "'$mysqlTmpDir'",g' /etc/my.cnf

mysql_install_db --force

/etc/init.d/mysqld start
/etc/init.d/mysqld enable

read -s -p "Enter MySQL Root Password: " mysqlrootpassword
echo;

mysqladmin -u root password $mysqlrootpassword



### install MySQLi-Driver for PHP

opkg update
opkg install php5-mod-mysqli

sed -i 's,;extension=mysql.so,extension=mysqli.so,g' /etc/php.ini

echo "" >> /etc/php.ini
echo "[MySQLi]" >> /etc/php.ini
echo "mysql.allow_local_infile = On" >> /etc/php.ini
echo "mysql.allow_persistent = On" >> /etc/php.ini 
echo "mysql.cache_size = 2000" >> /etc/php.ini 
echo "mysql.max_persistent = -1" >> /etc/php.ini 
echo "mysql.max_links = -1" >> /etc/php.ini
echo "mysql.default_port = 3306" >> /etc/php.ini
echo "mysql.default_socket = /tmp/run/mysqld.sock" >> /etc/php.ini
echo "mysql.default_host = 127.0.0.1" >> /etc/php.ini
echo "mysql.default_user = root" >> /etc/php.ini
echo "mysql.default_password = " >> /etc/php.ini
echo "mysql.connect_timeout = 60" >> /etc/php.ini
echo "mysql.trace_mode = Off" >> /etc/php.ini

# restarting mysqld
/etc/init.d/mysqld restart
# restarting uhhtpd to make sure, no cached anything is present anymore
/etc/init.d/uhttpd restart

### get the current source for linino into a tmp dir
if [[ ! -d /mnt/sda1/.install-wetbot ]]; then

	mkdir /mnt/sda1/.install-wetbot;
	
fi


echo "Done!";
echo;
