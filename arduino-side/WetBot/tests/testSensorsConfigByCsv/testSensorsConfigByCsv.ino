/**
 * Test for retrieving the Sensors-Config via an CSV file delivered by
 * the linino-side of this ArduinoYún
 */

#include <FileIO.h>

#include "Sensors.h"

#include "InitSensorsByCsv.h"

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Bridge.begin();
  Serial.begin(9600);
  while(!Serial);
  digitalWrite(13, LOW);
  
  initSensorsByCsv();

}

void loop() {
  // put your main code here, to run repeatedly:

}

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


