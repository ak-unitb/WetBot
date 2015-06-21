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
    bool isActive;

    Sensor () {}; // null constructor
    Sensor (int, uint32_t, uint32_t, bool); // id, frequencyThresholdTooDry, frequencyThresholdTooWet, isActive
    void setGradeOfDrynessByFrequency(uint32_t);
    const char* getGradeOfDrynessLiterally();
    bool justChangedGradeOfDryness();
};

Sensor initSensors();

Sensor getNextSensor(Sensor sensor);

extern Sensor SENSORs[6];

#endif

