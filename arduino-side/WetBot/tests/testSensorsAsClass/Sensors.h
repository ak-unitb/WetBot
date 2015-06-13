#ifndef Sensors_h
#define Sensors_h

#include <Arduino.h>

class Sensor {
  public:
    int id;
    int activeLedIdx = -1;
    uint32_t frequency = 0;
    uint8_t pinNumber;
    int gradeOfDryness = 1;
    int previousGradeOfDryness = 1;
    uint32_t frequencyThresholdTooDry;
    uint32_t frequencyThresholdTooWet;

    Sensor () {}; // null constructor: should be used but for tests!
    Sensor (int, uint32_t, uint32_t); // id, frequencyThresholdTooDry, frequencyThresholdTooWet
    void setGradeOfDrynessByFrequency(uint32_t);
    char* getGradeOfDrynessLiterally();
    //void setPreviousGradeOfDryness(int);
};

Sensor initSensors();

Sensor getNextSensor(Sensor sensor);
//Sensor getPreviousSensor(Sensor sensor); // obsolete?

extern Sensor SENSORs[5];
////extern Sensor previousSensor;
extern Sensor activeSensor;

#endif

