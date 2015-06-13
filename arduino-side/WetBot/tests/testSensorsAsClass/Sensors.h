#ifndef Sensors_h
#define Sensors_h

#include <Arduino.h>

class Sensor {
  public:
    int id;
    uint8_t sensorPinNumber;
    uint8_t relayPinNumber;
    uint32_t frequency;
    int gradeOfDryness; /* 1: tooWet (really less dryness); 2: wet (some more dryness), 3: tooDry (a lot of dryness)  */
    int previousGradeOfDryness;
    uint32_t frequencyThresholdTooDry;
    uint32_t frequencyThresholdTooWet;

    Sensor () {}; // null constructor: should NOT be used but for tests!
    Sensor (int, uint32_t, uint32_t); // id, frequencyThresholdTooDry, frequencyThresholdTooWet
    void setGradeOfDrynessByFrequency(uint32_t);
    const char* getGradeOfDrynessLiterally();
    bool justChangedGradeOfDryness();
};

Sensor initSensors();

Sensor getNextSensor(Sensor sensor);

extern Sensor SENSORs[2];

#endif

