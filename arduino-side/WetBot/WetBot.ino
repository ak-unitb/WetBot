/*
WetBot by count0 (Mr. Software) and Taquma (Mr. Hardware)

Last change: 21. Juni 2015 by count0

basiert auf:
 
 * FreqCount
 * https://github.com/PaulStoffregen/FreqCount
 * Example with serial output:
 * http://www.pjrc.com/teensy/td_libs_FreqCount.html
 *
 * This example code is in the public domain.

Sensors Input on Pin 12      / Arduino Yún
Sensors VCC   on Pin A0 - A6 / Arduino Yún
Valves VCC    on Pin 4 - 9   / Arduino Yún
 
Der Sketch verwendet 21.906 Bytes (76%) des Programmspeicherplatzes. Das Maximum sind 28.672 Bytes.
Globale Variablen verwenden 956 Bytes (37%) des dynamischen Speichers, 1.604 Bytes für lokale Variablen verbleiben.
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
//#include <Process.h> // already included...
#include "SaveSensorData.h"

// including Sensors -> checked per test!
#include "Sensors.h"

// include for sensosr config by CSV
//#include <Process.h> // already included...
#include "InitSensorsByCsv.h"

// include YùnApi -> checked
#include <YunServer.h>
#include <YunClient.h>
#include "YunApi.h"

// variable for saving sensor data
char currentComment[14] = "";

Sensor SENSORs[6]; // keep this variable in sync with the one in Sensors.h
Sensor activeSensor;

// variables for loop control
//const long waitIntervallForRead = 86400000; // in millisecs // 24 * 60 * 60 * 1000 => one day
//const long waitIntervallForRead = 600000; // in millisecs // ten minutes ist best. Any sensor is triggend once in an hour
const long waitIntervallForRead = 60000; // in millisecs // 1 * 60 * 1000 => one minute for changing the sensor ... for debugging

unsigned long previousMillis = 0;        // will store last time millies were updated


void setup() {

  Serial.begin(9600);	// Initialize the Serial
  pinMode(13, OUTPUT); // initialize the LED pin as an output

  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);

  // setup Brigde for InitSensorsByCSV, SaveSensorData and YùnTimeSync
  Bridge.begin();

  delay(200);

  initAndSyncTime();

  initSaveSensorData();

  activeSensor = initSensorsByCsv();

  initYunServer();

  FreqCount.begin(1000);

  delay(1000);
  
} // end void setup


void loop() {
  
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis >= waitIntervallForRead) {

    previousMillis = currentMillis;   

    if (FreqCount.available()) {
  
      activeSensor.setGradeOfDrynessByFrequency(FreqCount.read());

    }
  
    // case of ERROR!:
    if (activeSensor.frequency == 0) {
  
      // for security reasons switch off the relay - in case i was on before, we stop watering!
      digitalWrite(activeSensor.relayPinNumber, LOW);
      strcpy(currentComment, "error");
  
    } else {
  
      if (activeSensor.justChangedGradeOfDryness()) {
        if (activeSensor.gradeOfDryness >= 3) { // it gets too dry
          digitalWrite(activeSensor.relayPinNumber, HIGH);
          strcpy(currentComment, "change2tooDry");
        } else if (activeSensor.gradeOfDryness <= 1 ) { // it gets too wet
          digitalWrite(activeSensor.relayPinNumber, LOW);
          strcpy(currentComment, "change2TooWet");
        } else {
          strcpy(currentComment, "change");
        }
      }
    }

    // saving the data in DB
    insertSensorDataByPhpCli(activeSensor.frequency, activeSensor.gradeOfDryness, currentComment, (1 + activeSensor.id));

    // resetting currentComment
    strcpy(currentComment, "");

    // finally switch to the next sensor
    activeSensor = getNextSensor(activeSensor);

    delay(200);
  }

  listenApiRequests();

  delay(1000);

} // end void loop
