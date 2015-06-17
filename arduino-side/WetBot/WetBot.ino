/*
YunFreq by count0 (Mr. Software) and Taquma (Mr. Hardware)
06. Juni 2015
Letzte Änderung: 10. Juni 2015 by tq

basiert auf:
 
 * FreqCount
 * https://github.com/PaulStoffregen/FreqCount
 * Example with serial output:
 * http://www.pjrc.com/teensy/td_libs_FreqCount.html
 *
 * This example code is in the public domain.

 Sensors Input auf Pin 12 / Arduino Yún
 Sensors VCC   auf Pin A0 - A6 / Arduino Yún
 
Der Sketch verwendet 23.722 Bytes (82%) des Programmspeicherplatzes. Das Maximum sind 28.672 Bytes.
Globale Variablen verwenden 1.413 Bytes (55%) des dynamischen Speichers, 1.381 Bytes für lokale Variablen verbleiben.
Das Maximum sind 2.560 Bytes.


Vielen Dank an die Arduino-Community und alle Freundinnen und Freunde des Open-Source-Gedankens.

Copy me, I want to travel...

 */
#include <FreqCount.h>

// includes for YunTimeSync.ino -> checked per test!
#include <Process.h>
#include <Time.h>
#include "YunTimeSync.h"

// includes for SaveSensorData.ino -> checked per test!
#include <Process.h>
#include "SaveSensorData.h"

// including Sensors -> checked per test!
#include "Sensors.h"

// include YùnAppi
#include <YunServer.h>
#include <YunClient.h>
#include "YunApi.h"


char currentComment[7] = "";
Sensor SENSORs[2];
Sensor activeSensor;

//const long waitIntervallForRead = 86400000; // in millisecs // 24 * 60 * 60 * 1000 => one day
//const long waitIntervallForRead = 3600000; // in millisecs // one hour per each sensor is best!! (cause: 60 is dividable by 6, 5, 4 , 3, 2 and 1 - so each acessible count of sensors... - AND: a quite reasanable interval for real life measurments... ;) )
const long waitIntervallForRead = 60000; // in millisecs // 1 * 60 * 1000 => one minute for changing the sensor ... for debugging




void setup() {

  Serial.begin(9600);	// Initialize the Serial
  pinMode(13, OUTPUT); // initialize the LED pin as an output

  // waiting for serial just for debugging purposes
  while (!Serial) {
    // wait for serial port to connect.
    digitalWrite(13, HIGH);
  }
  Serial.println("Serial is available.");
  Serial.println(" ");
  digitalWrite(13, LOW); // Serial now is available, switching off the led

  Serial.println("***             YunFreq            *** ");
  Serial.println("***       by count0/tq 6/2015      *** ");
  Serial.println("***  23.722 Bytes of 28.672 (82%)  *** ");
  Serial.println("");

  delay(200);

  Serial.print("will initialize the time sync");
  initAndSyncTime();
  Serial.println(" .... DONE!");

  Serial.print("will initialize the saving of sensor data");
  initSaveSensorData();
  Serial.println(" .... DONE!");

  Serial.print("will initialize the SENSORs");
  activeSensor = initSensors();
  Serial.println(" .... DONE!");

  Serial.print("will initialize the YunAPI");
  initYunServer();
  // for testing the YunServer
  pinMode(6, OUTPUT);
  Serial.println(" .... DONE!");  

  Serial.print("Measuring starts for first sensor with id: ");
  Serial.println(activeSensor.id);
  FreqCount.begin(1000);

  delay(1000);
  
} // end void setup


void loop() {

  if (FreqCount.available()) {

    activeSensor.setGradeOfDrynessByFrequency(FreqCount.read());

    Serial.print("Sensor: ");
    Serial.print(activeSensor.id);
    Serial.print(" -> frequency: ");
    Serial.print(activeSensor.frequency);
    Serial.print(" Hz => gradeOfDryness: ");
    Serial.print(activeSensor.gradeOfDryness);
    Serial.print(" @ ");
    Serial.println(digitalClockDisplay());
  }

  // case of ERROR!:
  if (activeSensor.frequency == 0) {

    // for security reasons switch off the relay - in case i was on before, we stop watering!
    digitalWrite(activeSensor.relayPinNumber, LOW);
    strcpy(currentComment, "error");

    Serial.println("Error: no signal.");

  } else {

    if (activeSensor.justChangedGradeOfDryness()) {
      if (activeSensor.gradeOfDryness >= 3) { // it gets too dry
        Serial.println();
        Serial.print("Change: Start watering for sensor.id: ");
        Serial.println(activeSensor.id);
        digitalWrite(activeSensor.relayPinNumber, HIGH);
      } else if (activeSensor.gradeOfDryness <= 1 ) { // it gets too wet
        Serial.println();
        Serial.print("Change Stop watering for sensor.id: ");
        Serial.println(activeSensor.id);
        digitalWrite(activeSensor.relayPinNumber, LOW);
      }
      strcpy(currentComment, "change");
    }

  }

  // saving the data in DB
  insertSensorDataByPhpCli(activeSensor.frequency, activeSensor.gradeOfDryness, currentComment, (1 + activeSensor.id));

  // resetting currentComment
  strcpy(currentComment, "");

  // finally switch to the next sensor
  activeSensor = getNextSensor(activeSensor);
  
  listenApiRequests();

  delay(waitIntervallForRead);

} // end void loop

