

//enum STR2INT_ERROR { SUCCESS, OVERFLOW, UNDERFLOW, INCONVERTIBLE };
//STR2INT_ERROR str2int (long &i, char const *s, int base);

// variables for sensors
const char* getFileContentCommand = "/bin/cat /mnt/sda1/arduino/sensors.csv";

Sensor initSensorsByCsv();
