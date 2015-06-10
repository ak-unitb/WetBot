/*
YunFreq by count0 (Mr. Software) and Taquma (Mr. Hardware)
06. Juni 2015
Letzte Änderung: 10. Juni 2015 by tq

basiert auf:
 
 * FreqCount - Example with serial output
 * http://www.pjrc.com/teensy/td_libs_FreqCount.html
 *
 * This example code is in the public domain.

 Sensors Input auf Pin 12 / Arduino Yún
 Sensors VCC   auf Pin 14 - 19 / Arduino Yún
 
 Fuer die Lauffähigkeit auf dem Yun ist in FreqCount/util/FreqCountTimers.h
 die Zeile 556, in der das TimerCounterControlRegister2 auf 0 gesetzt wird, auskommentiert:
 556 	//TCCR2B = 0;
 Der Befehl will Timer/Counter2 stoppen, aber offenbar besitzt der 32U4 nur TimerCounter0,1,3. 
 (siehe Atmel 32U4-Datasheet, S.83, Kapitel 12)
 FreqCountTimers.h wurde umbenannt in FreqCountTimersTq.h und muß im util-Verzeichnis 
 der FreqCount-Library die alte .h-Datei ersetzen.
 Dafür müssen im Code wahrscheinlich noch ein paar Zeiger umgebogen werden oder wir benennen die ganze Library neu...
 31.05. und 10.6.2015 / modified by tq

Der Sketch verwendet 21.888 Bytes (76%) des Programmspeicherplatzes. Das Maximum sind 28.672 Bytes.
Globale Variablen verwenden 1.471 Bytes (57%) des dynamischen Speichers, 1.089 Bytes für lokale Variablen verbleiben. 
Das Maximum sind 2.560 Bytes.

Vielen Dank an die Arduino-Community und alle Freundinnen und Freunde des Open-Source-Gedankens.

Copy me, I want to travel...

 */
#include <FreqCount.h>
#include <Process.h>
#include <Time.h>
#include <Wire.h>

const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

const int SENSOR_1  = A0;
const int SENSOR_2  = A1;
const int SENSOR_3  = A2;
const int SENSOR_4  = A3;
const int SENSOR_5  = A4;
const int SENSOR_6  = A5;
// const int SENSORs[6] = { SENSOR_1, SENSOR_2, SENSOR_3, SENSOR_4, SENSOR_5, SENSOR_6 };
const int SENSORs[2] = { SENSOR_1, SENSOR_2 };

int previousSensorNumber = -1;
int currentSensorNumber = -1;

                               // || Zustand            || count0 || Tacuma ||
const int LEDdusty = 4;        // |  staubig         => |  rot    |  rot2   |
const int LEDdry = 5;          // |  trocken         => |  gelb   |  rot1   |
const int LEDhumid = 6;        // |  feucht          => |  gruen  |  gelb   |
const int LEDwet = 7;          // |  nass            => |  blau   |  gruen  |
const int LEDjustWatered = 8;  // |  frisch gegossen => |  weiss  |  blau   |

const int LEDs[5]= { LEDjustWatered, LEDwet, LEDhumid, LEDdry, LEDdusty };

int previouslyHighlightedLedNumberOfSensor[(sizeof(SENSORs)/sizeof(int))] ={};
int currentlyHighlightedLedNumberOfSensor[(sizeof(SENSORs)/sizeof(int))] = {};

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
  Serial.println("***  21.888 Bytes Bytes of 28.672  *** ");
  Serial.println("");

  delay(200);

  Serial.println("will initialize the time");
  initAndSyncClock();

  Serial.println("will initialize the LEDs");
  initLeds();

  Serial.println("will initialize the SENSORs");
  initSensors();

  Serial.println("Messung beginnt.");
  FreqCount.begin(1000);
  Serial.print("waitIntervallForRead: ");
  Serial.println(waitIntervallForRead);
  delay(1000);
  
} // end void setup


void loop() {

  if (FreqCount.available()) {
    currentFrequency = FreqCount.read();
    Serial.print("Sensor: ");
    Serial.print(previousSensorNumber);
    Serial.print(" -> frequency: ");
    Serial.print(currentFrequency);
    Serial.print(" Hz => gradeOfDryness: ");
    Serial.print(getGradeOfDrynessByFrequency(currentFrequency));
    Serial.print(" @ ");
    digitalClockDisplay();
  }

  // After reading the Frequency after the delay of this loop we switch to the next SENSOR
  currentSensorNumber = getNextSensorNumber();
  if (previousSensorNumber > -1) {
    digitalWrite(SENSORs[previousSensorNumber], LOW);
  }
  digitalWrite(SENSORs[currentSensorNumber], HIGH);

  if (currentFrequency == 0) {

    Serial.println("Error: no signal.");
    digitalWrite(LEDs[previouslyHighlightedLedNumberOfSensor[previousSensorNumber]], LOW);
    currentlyHighlightedLedNumberOfSensor[previousSensorNumber] = 4; // TODO: get max grade of dryness ...
    digitalWrite(LEDs[currentlyHighlightedLedNumberOfSensor[previousSensorNumber]], HIGH);
    strcpy(currentComment, "error");
    // saving current led number of sensor for next iterations
    previouslyHighlightedLedNumberOfSensor[previousSensorNumber] = currentlyHighlightedLedNumberOfSensor[previousSensorNumber];

  } else {

    // keeping the current grade of dryness for sensor
    currentlyHighlightedLedNumberOfSensor[previousSensorNumber] = getGradeOfDrynessByFrequency(currentFrequency);

    if (previouslyHighlightedLedNumberOfSensor[currentSensorNumber] > -1) {
      digitalWrite(LEDs[previouslyHighlightedLedNumberOfSensor[currentSensorNumber]], LOW);
    }

    if (currentlyHighlightedLedNumberOfSensor[previousSensorNumber] != previouslyHighlightedLedNumberOfSensor[previousSensorNumber]) {

      Serial.print("####--- ! Dryness changed for Sensor: ");
      Serial.print(previousSensorNumber);
      Serial.print(" to: ");
      Serial.println(currentlyHighlightedLedNumberOfSensor[previousSensorNumber]);
      
      strcpy(currentComment, "change");

    }
    digitalWrite(LEDs[currentlyHighlightedLedNumberOfSensor[previousSensorNumber]], HIGH);

    // saving current led number of sensor for next iterations
    previouslyHighlightedLedNumberOfSensor[previousSensorNumber] = currentlyHighlightedLedNumberOfSensor[previousSensorNumber];
  }

  // saving the data in DB
  executeMysqlInsert(currentFrequency, currentlyHighlightedLedNumberOfSensor[previousSensorNumber], currentComment, (1 + previousSensorNumber));
  strcpy(currentComment, "");

  // saving current sensor number for next iteration
  previousSensorNumber = currentSensorNumber;

  delay(waitIntervallForRead);
} // end void loop



// so to say, private methods ;)
void initLeds() {

  // init LEDs status for sensor reflecting variables
  for (i = 0; i < (sizeof(SENSORs)/sizeof(int)); i++) {
    previouslyHighlightedLedNumberOfSensor[i] = -1;
    currentlyHighlightedLedNumberOfSensor[i] = -1;
  }

  Serial.print("LED-Tests: ");
  delay(800);
  setupLeds();
  delay(300);
  testLedsInOrder(300);
  Serial.println(" Done!\n");
}

void setupLeds() {
  for (i = 0; i < (sizeof(LEDs)/sizeof(int)); i++) {

    pinMode( LEDs[i], OUTPUT );
    
    digitalWrite(LEDs[i], HIGH);
    delay(500);
    digitalWrite(LEDs[i], LOW);
  }
}

void testLedsInOrder(int interruptionTime) {
  int lastPin = -1;
  for (i = 0; i <= (sizeof(LEDs)/sizeof(int)); i++) {
    if ( lastPin != -1 ) {
      digitalWrite(lastPin, LOW);
    }
    if ( i != (sizeof(LEDs)/sizeof(int))) { // not the last iteration...
      digitalWrite( LEDs[i], HIGH );
      lastPin = LEDs[i];
    }
    delay(interruptionTime);
  }
}

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

void initSensors() {

  for (i = 0; i < (sizeof(SENSORs)/sizeof(int)); i++) {
    pinMode( SENSORs[i], OUTPUT );
  }

  // switching to the first sensor:
  Serial.print("switching to the first sensor: ");
  Serial.print((1 + previousSensorNumber));
  Serial.print(" (");
  Serial.print(SENSORs[previousSensorNumber]);
  Serial.println(")");
  previousSensorNumber = 0;
  digitalWrite(SENSORs[previousSensorNumber], HIGH);
}

int getNextSensorNumber() {

  if (previousSensorNumber >= (sizeof(SENSORs)/sizeof(int)) - 1) {
    return 0;
  }
  return 1 + previousSensorNumber;
}


// execute a mysql-query via systemCall (provided by Bridge)
void executeMysqlInsert(long frequency, int gradeOfDryness, char* comment, int sensorNumber) {
  // INSERT INTO `moisture`.`sensor_datas` (`frequency`, `grade_of_dryness`, `comment`, `sensor_id`) VALUES (<long frequency>, <int gradeOdDryness>, <string comment [error|change]>, <int sensorNumber>);
  char insertTpl[155] = "mysql -uroot -parduino -e'INSERT INTO `moisture`.`sensor_datas` (`frequency`, `grade_of_dryness`, `comment`, `sensor_id`) VALUES (%lu, %d, \"%s\", %d);'"; // 9 chars to replace
  char insertStatement[160];
  int resultStringLength = sprintf(insertStatement, insertTpl, frequency, gradeOfDryness, comment, sensorNumber); // 6+1+6+2
  if (resultStringLength > 160 || resultStringLength < 0) {
    Serial.println("ERROR: function executeMysqlInsert: Buffer-Overflow while concatinating mysql-statement!");
  } else {
    //Serial.println(insertStatement);
    Process p;
    p.runShellCommand(insertStatement);
    // do nothing until the process finishes, so you get the whole output:
    while(p.running()) {
      digitalWrite(13, HIGH); // wait for Serial to connect.
    };
    digitalWrite(13, LOW);
    // mysql -e doesn't has any output, so we needn't to read anything... but anyway:
    while (p.available()) {
      int result = p.parseInt();   // look for an integer
    }
  }
}


void initAndSyncClock() {
  
  // thie makes this function clallable just once at the very beginning
  setSyncProvider( requestTimeSyncFromYunSide );  //set function to call when sync required
  setSyncInterval( (uint32_t)84960 ); // choosed (24 * 60 * 59) secs; a day has 86400  // argument is maximal: 4.294.967.295
}

time_t requestTimeSyncFromYunSide() {
  unsigned long pctime;
  char pctimeCharBuf[11] = "";
  Process p;

  p.runShellCommand("/bin/date +%s");
  
  // do nothing until the process finishes, so you get the whole output:
  while (p.running());

// Read command output. runShellCommand() should have passed "Signal: xx&":
  int i = 0;
  while (p.available() > 0) {
    char c = p.read();
    if (isdigit(c)) {
      pctimeCharBuf[i] = c;
    }
    //Serial.print(c);
    i += 1;
  }
  //Serial.println(;
  //Serial.print("pcCharBuf: ");
  //Serial.println(pctimeCharBuf);

  char *junk;
  pctime = strtol(pctimeCharBuf, &junk, 10);
  if (strlen(junk) > 0) { // systemcall response from yun side contains unexpected characters
    pctime = DEFAULT_TIME; // fall back to defined const fallback @see above
  }
  Serial.print("requestTimeSyncFromYunSide => read pctime is: ");
  Serial.println(pctime);

  return pctime;
}

void digitalClockDisplay() {
  // digital clock display of the time
  printDigits(day(), false);
  Serial.print(".");
  printDigits(month(), false);
  Serial.print(".");
  Serial.print(year()); 
  Serial.print(" ");
  printDigits(hour(), false);
  printDigits(minute(), true);
  printDigits(second(), true);
  Serial.print(" UTC");

  Serial.println(); 
}

void printDigits(int digits, bool colon){
  // utility function for digital clock display: prints preceding colon and leading 0
  if (colon) {
    Serial.print(":");
  }
  if (digits < 10) {
    Serial.print('0');
  }
  Serial.print(digits);
}

