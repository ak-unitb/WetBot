
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

      /* * /
      Serial.println();
      Serial.print("Value for line: ");
      Serial.print(lineCounter);
      Serial.print(" part: ");
      Serial.print(partsCounter);
      Serial.print(" => value: ");
      Serial.println(csvString);

      Serial.print("to int: ");
      /* */
      char *end;
      csvValue = strtol(csvString.c_str(), &end, 10);
      //Serial.println(csvValue);

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
        printSensor(SENSORs[lineCounter]);
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

  Serial.print("current free RAM: ");
  Serial.println(getFreeRam());

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

/* */
// for debugging purposes ... @TODO: remove me, if sketch is working!
void printSensor(Sensor sensor) {
  Serial.print("Sensor{id: ");
  Serial.print(sensor.id);
  Serial.print(", sensorPinNumber: ");
  Serial.print(sensor.sensorPinNumber);
  Serial.print(", relayPinNumber: ");
  Serial.print(sensor.relayPinNumber);
  Serial.print(", frequency: ");
  Serial.print(sensor.frequency);
  Serial.print(", gradeOfDryness: ");
  Serial.print(sensor.gradeOfDryness);
  Serial.print(" (");
  Serial.print(sensor.getGradeOfDrynessLiterally());
  Serial.print(")");
  Serial.print(", previousGradeOfDryness: ");
  Serial.print(sensor.previousGradeOfDryness);
  Serial.print(", frequencyThresholdTooDry: ");
  Serial.print(sensor.frequencyThresholdTooDry);
  Serial.print(", frequencyThresholdTooWet: ");
  Serial.print(sensor.frequencyThresholdTooWet);
  Serial.print(", active: ");
  Serial.print(sensor.isActive);
  Serial.println("};");
}
/* */
