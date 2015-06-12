#ifndef Sensors_h
#define Sensors_h

#include <Arduino.h>

struct sensor_t {
  int id;
  int activeLedIdx;
  uint32_t frequency;
  uint8_t pinNumber;
  int gradeOfDryness;
  int previousGradeOfDryness;
  uint32_t frequencyThresholdTooDry;
  uint32_t frequencyThresholdTooWet;
};

sensor_t initSensors();

sensor_t getNextSensor(sensor_t sensor);
sensor_t getPreviousSensor(sensor_t sensor);
void printSensor(sensor_t sensor);

extern sensor_t SENSORs[5];

#endif
