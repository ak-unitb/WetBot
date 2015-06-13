
// including Sensors
#include "Sensors.h"

Sensor SENSORs[5];
Sensor activeSensor;

int i = 0;

void setup() {

  Serial.begin(9600);	// Initialize the Serial
  pinMode(13, OUTPUT); // initialize the LED pin as an output

  // waiting for serial just for debugging purposes
  while (!Serial) {
    // wait for serial port to connect. Needed for Leonardo only
    digitalWrite(13, HIGH); // wait for Serial to connect.
  }
  Serial.println("Serial is available.");
  Serial.println(" ");
  digitalWrite(13, LOW); // Serial now is available, switching of the led
  
  Serial.println("will initialize the SENSORs");
  activeSensor = initSensors();
  
}

void loop() {

  printSensor(activeSensor);

  if (activeSensor.id == 4) {
    Serial.println();
  }

  activeSensor = getNextSensor(activeSensor);
  
  uint32_t nextFrequency = ( (uint32_t)1000 * (uint32_t)i ) + (uint32_t)activeSensor.gradeOfDryness;
  activeSensor.setGradeOfDrynessByFrequency(nextFrequency);

  delay(2000);
  i++;
}

void printSensor(Sensor sensor) {
  Serial.print("Sensor{id: ");
  Serial.print(sensor.id);
  Serial.print(", activeLedIdx: ");
  Serial.print(sensor.activeLedIdx);
  Serial.print(", frequency: ");
  Serial.print(sensor.frequency);
  Serial.print(", pinNumber: ");
  Serial.print(sensor.pinNumber);
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
  Serial.println("};");
}
