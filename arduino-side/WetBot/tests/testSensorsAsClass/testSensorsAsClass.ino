/**
 *
 * @TODO: after a decent number of iterations, this test produces an BufferOverflow for uint32_t
 *        guess, it's the trial to assign a negative value... FIXME!
 */


// including Sensors
#include "Sensors.h"

Sensor SENSORs[2];
Sensor activeSensor;

int i = 0;
int32_t currentAcceleration = -1;

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
  
  Serial.print("will initialize the SENSORs");
  activeSensor = initSensors();
  Serial.println(" .... DONE!");
  
  Serial.println();
  for (int ii = 0; ii < 2; ii++) {
    printSensor(SENSORs[ii]);
  }
  Serial.println();
  Serial.println();
}

void loop() {
  //printSensor(activeSensor);

  if (activeSensor.id == 4) {
    //Serial.println();
  }

  // read frequency, save and made decision to stop or start watering if necessary
  Serial.print("frequency calcultaion: ");
  Serial.print(activeSensor.frequency);
  Serial.print(" + ( 2000 * ");
  Serial.print(currentAcceleration);
  Serial.println(" )");
  uint32_t nextFrequency = (uint32_t)( (uint32_t)activeSensor.frequency + ( (uint32_t)2000 * (int32_t)currentAcceleration ) );
  if (i < 2) {
    nextFrequency = (uint32_t)100000;
  }
  Serial.print("nextFrequency: ");
  Serial.println(nextFrequency);
  activeSensor.setGradeOfDrynessByFrequency(nextFrequency);

  if (activeSensor.justChangedGradeOfDryness()) {
    if (activeSensor.gradeOfDryness >= 3) { // it gets too dry
      Serial.println();
      Serial.print("Change: Start watering: ");
      /* */
      Serial.print("power on relayPinNumber: ");
      Serial.print(activeSensor.relayPinNumber);
      Serial.print(" for sensor.id: ");
      Serial.println(activeSensor.id);
      digitalWrite(activeSensor.relayPinNumber, HIGH);
      printSensor(activeSensor);
      /* */
    } else if (activeSensor.gradeOfDryness <= 1) { // it gets too wet
      Serial.println();
      Serial.print("Change Stop watering: ");
      /* */
      Serial.print("power off relayPinNumber: ");
      Serial.print(activeSensor.relayPinNumber);
      Serial.print(" for sensor.id: ");
      Serial.println(activeSensor.id);
      digitalWrite(activeSensor.relayPinNumber, LOW);
      printSensor(activeSensor);
      /* */
    }
  }

  activeSensor = getNextSensor(activeSensor);
  //Serial.print("current free RAM: ");
  //Serial.println(getFreeRam());
  /* */
  if (nextFrequency > 75000) {
    currentAcceleration = (int32_t)-1;
  } else if (nextFrequency < 5000) {
    currentAcceleration = (int32_t)+1;
  }
  Serial.print("frequency: ");
  Serial.print(nextFrequency);
  Serial.print(" currentAcceleration: ");
  Serial.println(currentAcceleration);
  Serial.println();

  //Serial.print(".");
  delay(500);
  i++;
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
  Serial.println("};");
}

int getFreeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
