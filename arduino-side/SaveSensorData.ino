
/**
 * This is about to be changed!
 * The linino-/Yùn-side already needs to know the db credentials for php.
 * So a php-cliscript should be installed outside of the webroot to
 * execute the INSERT-statement with the db credentials already readable for php.
 */

bool insertSensorDataByPhpCli(long frequency, int gradeOfDryness, char* comment, int sensorNumber) {

  // /usr/bin/php-cli /mnt/sda1/arduino/insertSensorData.php frequency=100000 grade_of_dryness=3 comment=change sensor_id=1
  char phpCliCallTpl[114] = "/usr/bin/php-cli /mnt/sda1/arduino/insertSensorData.php frequency=%lu grade_of_dryness=%d comment=%s sensor_id=%d";
  char phpCliCall[130];
  int resultStringLength = sprintf(phpCliCall, phpCliCallTpl, frequency, gradeOfDryness, comment, sensorNumber);
  if (resultStringLength > 160 || resultStringLength < 0) {
    // error ...
    return false;
  } else {
    Process p;
    p.runShellCommand(phpCliCall);
    // do nothing until the process finishes, so you get the whole output:
    while(p.running()) {
      digitalWrite(13, HIGH); // wait for Serial to connect.
    };
    digitalWrite(13, LOW);
    // mysql -e doesn't has any output, so we needn't to read anything... but anyway:
    while (p.available()) {
      int result = p.parseInt();   // look for an integer
    }
  }
  return true;
}
