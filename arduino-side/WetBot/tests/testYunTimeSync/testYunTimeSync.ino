
// includes for YunTimeSync.ino
#include <Process.h>
#include <Time.h>
#include "YunTimeSync.h"

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

  Serial.println("will initialize the time sync");
  initAndSyncTime();

}

void loop() {

  Serial.println("digitalClockDisplay: ");
  Serial.println(digitalClockDisplay());

/*  
  for (int i = 0; i < 61; i++) {
    Serial.print("toDigits for i: ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(toDigits(i, false));
  }

  for (int i = 0; i < 61; i++) {
    Serial.print("toDigits for i: ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(toDigits(i, true));
  }
*/
  Serial.println(".");
  delay(1000);
}

