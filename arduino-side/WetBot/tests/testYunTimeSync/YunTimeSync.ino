/*
  YùnTimeSync - Implemntation to sync the time from the linino-/Yùn-side
  Created by count0, June 11, 2015.
  Released into the public domain.
*/

bool timeSyncIsInitialized = false;

// main initialization function
void initAndSyncTime() {
  
  Bridge.begin();  // Initialize the Bridge (implemtation just step out, if Bridge is already started)  
  
  if (!timeSyncIsInitialized) {
    setSyncProvider( requestTimeSyncFromYunSide );  //set function to call when sync required
    
    uint32_t syncInterval = 2000; // choosed (24 * 60 * 59) secs; a day has 86400  // argument is maximal: 4.294.967.295
    
    setSyncInterval( syncInterval );
  }
}
// the heart - time syncing by requesting the linino/Yùn-side
time_t requestTimeSyncFromYunSide() {

  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  unsigned long pctime;
  char pctimeCharBuf[11] = "";
  Process p;

  p.runShellCommand("/bin/date +%s");
  
  // do nothing until the process finishes, so you get the whole output:
  while (p.running());

  // Read command output:
  int i = 0;
  while (p.available() > 0) {
    char c = p.read();
    if (isdigit(c)) {
      pctimeCharBuf[i] = c;
    }
    i += 1;
  }

  char *junk;
  pctime = strtol(pctimeCharBuf, &junk, 10);
  if (strlen(junk) > 0) { // systemcall response from yun side contains unexpected characters
    Serial.println("Fallback: take the default time Jan 1 2013");
    pctime = DEFAULT_TIME; // fall back to defined const fallback @see above
  }

  Serial.print("will return: ");
  Serial.println(pctime);
  return pctime;
}


/*
 * Currently just used for development and debugging purposes...
 */
//---- utility functions for time formatting

// display of the time
String digitalClockDisplay() {

  String result;
  Serial.print("digitalClockDisplay: after init the vars");

  char yearStr[5];
  sprintf(yearStr, "%d", year());
  Serial.println("digitalClockDisplay: after year to str");
  
  result = toDigits(day(), false) + "." +
           toDigits(month(), false) + "." +
           yearStr + " " +
           toDigits(hour(), false) +
           toDigits(minute(), true) +
           toDigits(second(), true) +
           " UTC";
  Serial.println("digitalClockDisplay: after concat date");

  return result;
}


// utility function for digital clock display: prints leading colon - if requested - and leading 0 if needed
String toDigits(int digits, bool colon){

  int charArrayLength = 3;
  String result = "";
  if (colon) {
    charArrayLength = 4;
    result = ":";
  }
  if (digits < 10) {
    result =  result + "0";
  }
  
  char digitStr[3];
  sprintf(digitStr, "%d", digits);
  
  result =  result + digitStr;
  
  return result;
}
/* */
