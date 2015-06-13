// includes for SaveSensorData.ino
#include <Process.h>
#include "SaveSensorData.h"

int i = 0;

void setup() {

  Serial.begin(9600);	// Initialize the Serial
  pinMode(13, OUTPUT); // initialize the LED pin as an output

  initSaveSensorData();

  // waiting for serial just for debugging purposes
  while (!Serial) {
    // wait for serial port to connect. Needed for Leonardo only
    digitalWrite(13, HIGH); // wait for Serial to connect.
  }
  Serial.println("Serial is available.");
  Serial.println(" ");
  digitalWrite(13, LOW); // Serial now is available, switching of the led
}

void loop() {
  Serial.println("before calling insertSensorDataByPhpCli");
  bool result = insertSensorDataByPhpCli((100000 + i), 3, "test", 3);
  Serial.println("after calling insertSensorDataByPhpCli");
  i++;
  Serial.print("i: ");
  Serial.print(i);
  Serial.print("insertSensorDataByPhpCli -> result is: ");
  Serial.println(result);
  delay(2000);
}
