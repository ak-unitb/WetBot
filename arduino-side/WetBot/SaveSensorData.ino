/**
 * This snippets saves data in the mysql-db by using a php-cli executable php script
 * Prerequisites:
 *   - installed php5-cli on the linino-/Yùn-side
 *   - the file insertSensorData.php on a sd-card located @ /mnt/sda1/arduino/
 */

void initSaveSensorData() {
  Bridge.begin();  // Initialize the Bridge (implemtation just step out, if Bridge is already started)  
}

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

    // Read command output:
    char charBuf[8] = "";
    int i = 0;
    while (p.available() > 0) {
      char c = p.read();
      charBuf[i] = c;
      i += 1;
    }
    // if script returned any error...
    if (strcmp(charBuf, "SUCCESS") != 0) {
      return false;
    }
  }
  return true;
}
