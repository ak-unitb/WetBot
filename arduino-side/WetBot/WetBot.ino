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
 
Der Sketch verwendet 21.010 Bytes (73%) des Programmspeicherplatzes. Das Maximum sind 28.672 Bytes.
Globale Variablen verwenden 1.456 Bytes (56%) des dynamischen Speichers, 1.377 Bytes für lokale Variablen verbleiben.
Das Maximum sind 2.560 Bytes.


Vielen Dank an die Arduino-Community und alle Freundinnen und Freunde des Open-Source-Gedankens.

Copy me, I want to travel...

 */
#include <FreqCount.h>

// includes for YunTimeSync.ino
#include <Process.h>
#include <Time.h>
#include "YunTimeSync.h"

// includes for SaveSensorData.ino -> chekced per test!
#include <Process.h>
#include "SaveSensorData.h"

// including Sensors
#include "Sensors.h"

                               // || Zustand            || count0 || Tacuma ||
const int LEDdusty = 4;        // |  staubig         => |  rot    |  rot2   |
const int LEDdry = 5;          // |  trocken         => |  gelb   |  rot1   |
const int LEDhumid = 6;        // |  feucht          => |  gruen  |  gelb   |
const int LEDwet = 7;          // |  nass            => |  blau   |  gruen  |
const int LEDjustWatered = 8;  // |  frisch gegossen => |  weiss  |  blau   |

const int LEDs[5]= { LEDjustWatered, LEDwet, LEDhumid, LEDdry, LEDdusty };


char currentComment[7] = "";



//const long waitIntervallForRead = 86400000; // in millisecs // 24 * 60 * 60 * 1000 => one day
//const long waitIntervallForRead = 3600000; // in millisecs // one hour per each sensor is best!! (cause: 60 is dividable by 6, 5, 4 , 3, 2 and 1 - so each acessible count of sensors... - AND: a quite reasanable interval for real life measurments... ;) )
const long waitIntervallForRead = 60000; // in millisecs // 1 * 60 * 1000 => one minute for changing the sensor ... for debugging

unsigned long currentFrequency;

int i = 0; // global iteration counter variable (take care, it's global ;) )



void setup() {

  Bridge.begin();	// Initialize the Bridge
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

  Serial.println("***             YunFreq            ***");
  Serial.println("***       by count0/tq 6/2015      *** ");
  Serial.println("***  21.120 Bytes of 28.672 (73%)  *** ");
  Serial.println("");

  delay(200);

  Serial.println("will initialize the time sync");
  initAndSyncTime();

  Serial.println("will initialize the LEDs");
  //initLeds();

  Serial.println("will initialize the SENSORs");
  activeSensor = initSensors();

  Serial.println("will initialize the SaveSensorData");  
  initSaveSensorData();

  Serial.println("Messung beginnt.");
  FreqCount.begin(1000);

  delay(1000);
  
} // end void setup


void loop() {

  if (FreqCount.available()) {

    activeSensor.frequency = FreqCount.read();
    activeSensor.gradeOfDryness = getGradeOfDrynessByFrequency(activeSensor.frequency);
    activeSensor.activeLedIdx = activeSensor.gradeOfDryness;

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

    digitalWrite(previousSensor.activeLedIdx, LOW);
    activeSensor.activeLedIdx = 4; // TODO: get max grade of dryness ...
    digitalWrite(activeSensor.activeLedIdx, HIGH);
    strcpy(currentComment, "error");

    Serial.println("Error: no signal.");

  } else {

    digitalWrite(previousSensor.activeLedIdx, LOW);

    if (activeSensor.gradeOfDryness != activeSensor.previousGradeOfDryness) {

      Serial.print("####--- ! Dryness changed for Sensor: ");
      Serial.print(activeSensor.previousGradeOfDryness);
      Serial.print(" to: ");
      Serial.println(activeSensor.gradeOfDryness);
      
      strcpy(currentComment, "change");

    }
    digitalWrite(activeSensor.activeLedIdx, HIGH);
  }

  // saving the data in DB
  insertSensorDataByPhpCli(activeSensor.frequency, activeSensor.gradeOfDryness, currentComment, (1 + activeSensor.id));

  // resetting currentComment
  strcpy(currentComment, "");

  // keeping the gradeOfDryness for the next time, this sensor is triggered
  activeSensor.previousGradeOfDryness = activeSensor.gradeOfDryness;

  // After reading the frequency and treatment before the delay of this loop we switch to the next SENSOR
  digitalWrite(activeSensor.pinNumber, LOW);
  previousSensor = activeSensor; // keep the previous in mind...
  activeSensor = getNextSensor(activeSensor);
  digitalWrite(activeSensor.pinNumber, HIGH);


  delay(waitIntervallForRead);
} // end void loop



// so to say, private methods ;)

/**
 * Formel generiert von www.arndt-bruenner.de/mathe/scripts/regr.htm
 * anhand der Dtaenreihe (5 LEDs):
 *  0       0
 *  15000   1
 *  50000   2
 *  100000  3
 *  200000  4
 *     => f(x) = -0 * x^2 + 0,000036 * x + 0,163636
 *  Für die Datenreihe (4 LEDs):
 *  0       0
 *  50000   1
 *  100000  2
 *  200000  3 
 *     => f(x) = -0 * x^2 + 0,000022 * x + 0,206379
 * Next trial: Formula isn't satisfying  - id need's to be somthign with ln(x); frequency first is divided by 1000 to get a simpler graph
 *  0       0  f(0) = 0 to f(1708) = 0
 *  16      1  f(1709) = 1 to f(18137) = 1
 *  32      2  f(18138) = 2 to f(74726) = 2
 *  96      3  f(74727) = 3 to f(132976) = 3
 *  160     4  f(132977) = 4
 *     => f(x) = 0.501611 * log(x) + 0.000059 * x^2
 */
int getGradeOfDrynessByFrequency(float freq) {
  //Serial.print("getGradeOfDrynessByFrequency: with freq: ");
  //Serial.print(freq);
  //int gradeOfDryness = round( ( 0.000022 * freq ) + 0.163636 );
  float x = freq / 1000;
  int gradeOfDryness = round( ( ( 0.501611 * log( x + 1) ) + ( 0.000059 * pow(x, 2) ) ) );
  //Serial.print(" and rounded result: ");
  //Serial.print(gradeOfDryness);
  if (gradeOfDryness >= (sizeof(LEDs)/sizeof(int))) {
    //Serial.println(" WARN: formula calculated incompatible result !!!!");
    gradeOfDryness = (sizeof(LEDs)/sizeof(int)) - 1;
  }
  //Serial.println("");

  return gradeOfDryness;
}

