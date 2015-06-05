/*
YunFreq 0.2a by Tacuma, count0
02. Juni 2015
basiert auf:
 
* FreqCount - Example with serial output
 * http://www.pjrc.com/teensy/td_libs_FreqCount.html
 *
 * This example code is in the public domain.

 Sensor Input auf Pin 12 / Arduino YUN
 Fuer die Lauffaehigkeit ist in der FreqCount.cpp oder so
 eine zeile 556 oder so auskommentiert. (irgendwas mit Timer2 oder so)
 31.05.2015 / modified by tq
 
Vielen Dank an die Arduino-Community und alle OpenSourc@s in the World!

 */
#include <FreqCount.h>
#include <Process.h>
#include <Time.h>
#include <Wire.h>

#define TIME_HEADER  'T'   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 
const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

//const int LEDrot2 = 4; // staubig
const int LEDrot = 5; // trocken
const int LEDgelb = 6; // feucht
const int LEDgruen = 7; // nass
const int LEDblau = 8; // nass / frisch gegossen

//const int LEDs[5] = { LEDgruen, LEDgelb, LEDorange, LEDrot1, LEDrot2 };
//const int LEDs[4] = { LEDgruen, LEDgelb, LEDorange, LEDrot1 };
const int LEDs[4] = { LEDblau, LEDgruen, LEDgelb, LEDrot };

int previouslyHighlightedLedNumber = -1;
int currentlyHighlightedLedNumber = -1;


const int SENSOR_1  = 14;
const int SENSOR_2  = 15;
const int SENSOR_3  = 16;
const int SENSOR_4  = 17;
const int SENSOR_5  = 18;
const int SENSOR_6  = 19;
const int[2] = { SENSOR_1, SENSOR_2 };



const long waitIntervallForRead = 60000;

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
  digitalWrite(13, LOW); // Serial now is available, switching of the led

  Serial.println("***  YunFreq 0.1  ***");
  Serial.println("***  by tq 6/2015 *** ");
  Serial.println("***  6.504 Bytes  *** ");
  Serial.println("");

  delay(200);

  Serial.println("will initialize the time");
  initAndSyncClock();

  delay(200);

  Serial.println("will initialize the LEDs");
  initLeds();

  FreqCount.begin(1000);
  Serial.println("Messung beginnt.");
  delay(1000);
  
}


void loop() {

  if (FreqCount.available()) {
    currentFrequency = FreqCount.read();
    Serial.print("Frequenz: ");
    Serial.print(currentFrequency);
    Serial.print(" Hz um ");
    digitalClockDisplay();
  }
  if (currentFrequency == 0) {
    Serial.println("Error: Kein Signal.");
  } else {

    currentlyHighlightedLedNumber = getGradeOfDrynessByFrequency(currentFrequency);
    if (currentlyHighlightedLedNumber != previouslyHighlightedLedNumber) {
      if (previouslyHighlightedLedNumber > -1) {

        digitalWrite(LEDs[previouslyHighlightedLedNumber], LOW);
      }

      Serial.print("####--- ! Dryness changed to ");
      Serial.println(currentlyHighlightedLedNumber);
      digitalWrite(LEDs[currentlyHighlightedLedNumber], HIGH);
      
      // saving current led for next iteration
      previouslyHighlightedLedNumber = currentlyHighlightedLedNumber;
    }/* else {
      Serial.print("."); // debugging ... 
    }*/
  }
  delay(waitIntervallForRead);
} // end void loop



// so to say, private methods ;)
void initLeds() {
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
 */
int getGradeOfDrynessByFrequency(float freq) {
  //Serial.print("getGradeOfDrynessByFrequency: with freq: ");
  //Serial.print(freq);
  int gradeOfDryness = round( ( 0.000022 * freq ) + 0.163636 );
  //Serial.print(" and rounded result: ");
  //Serial.print(gradeOfDryness);
  if (gradeOfDryness >= (sizeof(LEDs)/sizeof(int))) {
    Serial.println(" WARN: formula calculated incompatible result !!!!");
    gradeOfDryness = (sizeof(LEDs)/sizeof(int)) - 1;
  }
  //Serial.println("");

  return gradeOfDryness;
}

void initAndSyncClock() {
  
  // thie makes this function clallable just once at the very beginning
  setSyncProvider( requestTimeSyncFromYunSide );  //set function to call when sync required
  setSyncInterval( (uint32_t)(24 * 60 * 59) ); // choosed 84960 secs; a day has 86400  // argument is maximal: 4.294.967.295
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
  Serial.print("requestTimeSyncFromYunSidecd => read pctime is: ");
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
  Serial.print(hour());
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

