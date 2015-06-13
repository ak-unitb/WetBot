
#include <Arduino.h>
#include "Sensors.h"

Sensor::Sensor (int pId, uint32_t pfrequencyThresholdTooDry, uint32_t pfrequencyThresholdTooWet) {
  id = pId;
  pinNumber = (A0 + pId);
  frequencyThresholdTooDry = pfrequencyThresholdTooDry;
  frequencyThresholdTooWet = pfrequencyThresholdTooWet;
  /*
  // defaults:
  gradeOfDryness = 1;
  previousGradeOfDryness = 1;
  */
}

void Sensor::setGradeOfDrynessByFrequency(uint32_t pFrequency) {
  Serial.print("Sensor::setGradeOfDrynessByFrequency -> (before)previousGradeOfDryness: ");
  Serial.print(previousGradeOfDryness);
  Serial.print(" Shall be set to: ");
  Serial.println(gradeOfDryness);
  if (previousGradeOfDryness != gradeOfDryness) {
    Serial.println("Sensor::setGradeOfDrynessByFrequency -> in if ");
    int currentGradeOfDryness = gradeOfDryness;
    previousGradeOfDryness = currentGradeOfDryness; // saving the current value before setting a new one...
  }
  Serial.print("Sensor::setGradeOfDrynessByFrequency -> (after)previousGradeOfDryness: ");
  Serial.println(previousGradeOfDryness);

  frequency = pFrequency;
  if (pFrequency >= frequencyThresholdTooDry) {
    gradeOfDryness = 1;
  } else if (pFrequency <= frequencyThresholdTooWet) {
    gradeOfDryness = 3;
  } else {
    gradeOfDryness = 2;
  }
}
/*
void Sensor::setPreviousGradeOfDryness(int pGradeOfDryness) {
  previousGradeOfDryness = pGradeOfDryness;
}
*/

char* Sensor::getGradeOfDrynessLiterally() {
  if (gradeOfDryness == 2) {
    return "wet";    
  } else if (gradeOfDryness >= 3) {
    return "tooDry";
  } else if (gradeOfDryness >= 1) {
    return "tooWet";
  }
}

Sensor initSensors() {
  for (int i = 0; i < 5; i++) {
    // initialize the sensor struct
    
    Sensor sensor (i, (uint32_t)(50000 + i), (uint32_t)(14000 + i));
    
    SENSORs[i] = sensor;

    // initialize the sensors pin
    pinMode(SENSORs[i].pinNumber, OUTPUT);
  }

  digitalWrite(SENSORs[0].pinNumber, HIGH);
  return SENSORs[0];
}

Sensor getNextSensor(Sensor sensor) {
  if (sensor.id >= (sizeof(SENSORs) / sizeof(Sensor)) - 1) {
    return SENSORs[0];
  }
  return SENSORs[(1 + sensor.id)];
}
/* // obsolete?
Sensor getPreviousSensor(Sensor sensor) {
  if (sensor.id <= 0) {
    return SENSORs[((sizeof(SENSORs) / sizeof(Sensor)) - 1)];
  }
  return SENSORs[(sensor.id - 1)];
}
*/


