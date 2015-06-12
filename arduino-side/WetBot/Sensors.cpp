
#include "Sensors.h"

sensor_t SENSORs[5];

sensor_t initSensors() {
  for (int i = 0; i < 5; i++) {
    // initialize the sensor struct
    SENSORs[i].id = i;
    SENSORs[i].activeLedIdx = 4;
    SENSORs[i].frequency = 0;
    SENSORs[i].pinNumber = (A0 + i);
    SENSORs[i].gradeOfDryness = 4;
    SENSORs[i].previousGradeOfDryness = 4;
    SENSORs[i].frequencyThresholdTooDry = 50000;
    SENSORs[i].frequencyThresholdTooWet = 14000;

    // initialize the sensors pin
    pinMode(SENSORs[i].pinNumber, OUTPUT);
  }

  digitalWrite(SENSORs[0].pinNumber, HIGH);
  return SENSORs[0];
}

sensor_t getNextSensor(sensor_t sensor) {
  if (sensor.id >= (sizeof(SENSORs) / sizeof(sensor_t)) - 1) {
    return SENSORs[0];
  }
  return SENSORs[(1 + sensor.id)];
}

sensor_t getPreviousSensor(sensor_t sensor) {
  if (sensor.id <= 0) {
    return SENSORs[((sizeof(SENSORs) / sizeof(sensor_t)) - 1)];
  }
  return SENSORs[(sensor.id - 1)];
}

