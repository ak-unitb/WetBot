
#include <Arduino.h>
#include "Sensors.h"

Sensor::Sensor (int pId, uint32_t pfrequencyThresholdTooDry, uint32_t pfrequencyThresholdTooWet) {
  id = pId;
  sensorPinNumber = (A0 + pId); // using analog pins A0 to A5
  relayPinNumber = (4 + pId); // using digital pins from 4 to 9
  frequency = (uint32_t)1;
  gradeOfDryness = -1;
  previousGradeOfDryness = 1;
  frequencyThresholdTooDry = pfrequencyThresholdTooDry;
  frequencyThresholdTooWet = pfrequencyThresholdTooWet;
}

void Sensor::setGradeOfDrynessByFrequency(uint32_t pFrequency) {
  //Serial.print("Sensor::setGradeOfDrynessByFrequency -> (before)previousGradeOfDryness: ");
  //Serial.println(previousGradeOfDryness);
  previousGradeOfDryness = gradeOfDryness; // saving the current value before setting a new one...
  //Serial.print("Sensor::setGradeOfDrynessByFrequency -> (after)previousGradeOfDryness: ");
  //Serial.println(previousGradeOfDryness);

  frequency = pFrequency;
  /* 1: tooWet (really less dryness); 2: wet (some more dryness), 3: tooDry (a lot of dryness)  */
  if (pFrequency >= frequencyThresholdTooDry) {
    gradeOfDryness = 3;
  } else if (pFrequency <= frequencyThresholdTooWet) {
    gradeOfDryness = 1;
  } else {
    gradeOfDryness = 2;
  }
}

/* 1: tooWet (really less dryness); 2: wet (some more dryness), 3: tooDry (a lot of dryness)  */
const char* Sensor::getGradeOfDrynessLiterally() {
  if (gradeOfDryness <= 1) {
    return "tooWet";
  } else if (gradeOfDryness == 2) {
    return "wet";
  } else if (gradeOfDryness >= 3) {
    return "tooDry";
  }
}

bool Sensor::justChangedGradeOfDryness() {
  return previousGradeOfDryness != gradeOfDryness;
}

Sensor initSensors() {
  for (int i = 0; i < 2; i++) {
    // initialize the sensor struct

    Sensor sensor (i, (uint32_t)(50000 + i), (uint32_t)(14000 + i));

    SENSORs[i] = sensor;

    // initialize the sensors pin
    Serial.print(" initSensors: enabling output for sensorPin: ");
    Serial.print(SENSORs[i].sensorPinNumber);
    Serial.print(" for sensor.id: ");
    Serial.print(SENSORs[i].id);
    pinMode(SENSORs[i].sensorPinNumber, OUTPUT);
    // initialize the relays pin
    Serial.print(" initSensors: enabling output for relay: ");
    Serial.print(SENSORs[i].relayPinNumber);
    Serial.print(" for sensor.id: ");
    Serial.print(SENSORs[i].id);
    pinMode(SENSORs[i].relayPinNumber, OUTPUT);
  }

  // power on the first sensor
  digitalWrite(SENSORs[0].sensorPinNumber, HIGH);
  return SENSORs[0];
}

Sensor getNextSensor(Sensor sensor) {
  // power off the current sensor
  Serial.print("power off sensorPin: ");
  Serial.print(sensor.sensorPinNumber);
  Serial.print(" for sensor.id: ");
  Serial.println(sensor.id);
  digitalWrite(sensor.sensorPinNumber, LOW);
  // put the current status of this sensor into the array
  SENSORs[sensor.id] = sensor;

  Sensor nextSensor;
  if (sensor.id >= (sizeof(SENSORs) / sizeof(Sensor)) - 1) {
    nextSensor = SENSORs[0];
  } else {
    nextSensor =  SENSORs[(1 + sensor.id)];
  }

  // power on the next sensor
  Serial.print("power on sensorPin: ");
  Serial.print(nextSensor.sensorPinNumber);
  Serial.print(" for sensor.id: ");
  Serial.println(nextSensor.id);
  digitalWrite(nextSensor.sensorPinNumber, HIGH);
  return nextSensor;
}

