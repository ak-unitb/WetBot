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
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t

#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by Unix time_t as ten ASCII digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

// T1262347200  //noon Jan 1 2010

//const int LEDrot2 = 4; // staubig
const int LEDrot = 5; // trocken
const int LEDgelb = 6; // feucht
const int LEDgruen = 7; // nass
const int LEDblau = 8; // nass / frisch gegossen

//const int LEDs[5] = { LEDgruen, LEDgelb, LEDorange, LEDrot1, LEDrot2 };
//const int LEDs[4] = { LEDgruen, LEDgelb, LEDorange, LEDrot1 };
const int LEDs[4] = { LEDblau, LEDgruen, LEDgelb, LEDrot };

const long waitIntervallForRead = 60000;

unsigned long currentFrequency;

int i = 0; // global iteration counter variable (take care, it's global ;) )

int previouslyHighlightedLedNumber = -1;
int currentlyHighlightedLedNumber = -1;


void setup() {

  Serial.begin(57600);
  
  // waiting for serial just for debugging purposes
  pinMode(13, OUTPUT); // initialize the LED pin as an output
  while (!Serial) {
    // wait for serial port to connect. Needed for Leonardo only
    digitalWrite(13, HIGH); // wait for Serial to connect.
  }
  Serial.println("Serial is available.");
  digitalWrite(13, LOW); // Serial now is available, switching of the led

  setSyncProvider( requestSync);  //set function to call when sync required
  processTimeSync();
  while (timeStatus() != timeSet) {
     Serial.println("Unable to sync with Yùn side");
     digitalWrite(13, HIGH); // wait for Serial to connect.
  }
  Serial.println("RTC has set the system time");
  digitalWrite(13, LOW); // RTC has set the system time


  Serial.println("***  YunFreq 0.1  ***");
  Serial.println("***  by tq 6/2015 *** ");
  Serial.println("***  6.504 Bytes  *** ");
  Serial.println("");
  
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
  delay(300);
  blinkAllLeds();
  blinkAllLeds();
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

void blinkAllLeds() {
  for (i = 0; i <= (sizeof(LEDs)/sizeof(int)); i++) {
    digitalWrite( LEDs[i], HIGH );
  }
  delay(500);
  for (i = 0; i <= (sizeof(LEDs)/sizeof(int)); i++) {
    digitalWrite( LEDs[i], LOW );
  }
  delay(500);
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

time_t requestSync() {
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}

void processTimeSync() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013
  Process p;
  p.runShellCommand("date +%s");
  
  Serial.println("Waiting for runShellCommand to finish...");
  while (p.running()) {
    ;// do nothing until the process finishes, so you get the whole output:
  }
  Serial.println("runShellCommand finished.");
  while (p.available()) {
    pctime = p.parseInt();
    Serial.print("pctime: ");
    Serial.println(pctime);
     if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
       setTime(pctime); // Sync Arduino clock to the time received on the serial port
     }
  }
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.println(year()); 
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
