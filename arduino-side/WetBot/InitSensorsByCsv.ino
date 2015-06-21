
#include "Sensors.h"

Sensor initSensorsByCsv () {

  Process p;
  p.runShellCommand(getFileContentCommand);
  // do nothing until the process finishes, so you get the whole output:
  while (p.running());
  
  int id = 0;
  long frequencyThresholdTooWet = 0;
  long frequencyThresholdTooDry = 0;
  bool isActive = false;
  
  String csvString = "";
  long csvValue = 0;
  int lineCounter = 0;
  int partsCounter = 0;
  while (p.available() > 0) {
    char c = p.read();
    if (c == ',' || c == '\n') {

      char *end;
      csvValue = strtol(csvString.c_str(), &end, 10);

      if (partsCounter == 0) {
        id = (int)csvValue;
      } else if (partsCounter == 1) {
        frequencyThresholdTooWet = csvValue;
      } else if (partsCounter == 2) {
        frequencyThresholdTooDry = csvValue;
      } else if (partsCounter == 3) {
        isActive = (csvValue == 1) ? true : false;
      }

      csvString = "";

      if (c == ',') {
        partsCounter++;
      } else { // need to be linefoot

        Sensor sensor ((id - 1), (uint32_t)(frequencyThresholdTooWet), (uint32_t)(frequencyThresholdTooDry), isActive);
        SENSORs[lineCounter] = sensor;
        // initialize the pin of the sensor
        pinMode(SENSORs[lineCounter].sensorPinNumber, OUTPUT);
        // initialize the pin of the relay
        pinMode(SENSORs[lineCounter].relayPinNumber, OUTPUT);

        lineCounter++;
        partsCounter = 0;

        delay(100);
      }

    } else {
      //Serial.print(c);
      csvString += c;
    }

  } // end while
  p.flush();
  p.close();

  csvString = "";

  // power on the first active sensor
  if (SENSORs[0].isActive) {
    digitalWrite(SENSORs[0].sensorPinNumber, HIGH);
    delay(100);
    return SENSORs[0];
  } else {
    Sensor firstSensor = getNextSensor(SENSORs[0]);
    digitalWrite(firstSensor.sensorPinNumber, HIGH);
    delay(100);
    return firstSensor;
  }

}
