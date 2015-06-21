
#include <Arduino.h>
#include "Sensors.h"

Sensor::Sensor (int pId, uint32_t pfrequencyThresholdTooDry, uint32_t pfrequencyThresholdTooWet, bool pIsActive = false) {
  id = pId;
  sensorPinNumber = (A0 + pId); // using analog pins A0 to A5
  relayPinNumber = (4 + pId); // using digital pins from 4 to 9
  frequency = (uint32_t)1;
  gradeOfDryness = -1;
  previousGradeOfDryness = 1;
  frequencyThresholdTooDry = pfrequencyThresholdTooDry;
  frequencyThresholdTooWet = pfrequencyThresholdTooWet;
  isActive = pIsActive;
}

void Sensor::setGradeOfDrynessByFrequency(uint32_t pFrequency) {

  previousGradeOfDryness = gradeOfDryness; // saving the current value before setting a new one...

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

Sensor getNextSensor(Sensor sensor) {

  // power off the current sensor
  digitalWrite(sensor.sensorPinNumber, LOW);

  // put the current status of this sensor into the array
  SENSORs[sensor.id] = sensor;

  Sensor nextSensor;
  if (sensor.id >= (sizeof(SENSORs) / sizeof(Sensor)) - 1) {
    nextSensor = SENSORs[0];
  } else {
    nextSensor =  SENSORs[(1 + sensor.id)];
  }

  if (!nextSensor.isActive) { // @TODO: if no active sensor is configured this is an endless recursion!!!!
    //Serial.println("nextSensor isn't active, choose next one!");
    nextSensor = getNextSensor(nextSensor);
  }

  // power on the next sensor
  digitalWrite(nextSensor.sensorPinNumber, HIGH);
  return nextSensor;
}

