

bool timeSyncIsInitialized = false;

// main initialization function
void initAndSyncTime() {
  if (!timeSyncIsInitialized) {
    setSyncProvider( requestTimeSyncFromYunSide );  //set function to call when sync required
    setSyncInterval( (uint32_t)84960 ); // choosed (24 * 60 * 59) secs; a day has 86400  // argument is maximal: 4.294.967.295
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
    pctime = DEFAULT_TIME; // fall back to defined const fallback @see above
  }

  return pctime;
}


//---- utility functions for time formatting

// display of the time
char* digitalClockDisplay() {
  char* result;
  const char pattern[22] = "%s.%s.%d %s:%s:%s UTC";
  char *error =  new char[22]; // "Error formatting time";
  strncpy(error, "Error formatting time", sizeof("Error formatting time"));

  int resultLength = sprintf(
    result,
    pattern,
    toDigits(day(), false),
    toDigits(month(), false),
    year(),
    toDigits(hour(), false),
    toDigits(minute(), true),
    toDigits(second(), true)
  );
  if (resultLength > 4 || resultLength < 0) {
    return error;
  }
  return result;
}

// utility function for digital clock display: prints loading colon - if requested - and  0
char* toDigits(int digits, bool colon){

  char* result;
  char pattern[4];
  char *error = new char[4];
  strncpy(error, ":XX", sizeof(":XX"));
  
  strncpy(pattern, getDigitPattern(digits, colon).c_str(), sizeof(pattern));
  pattern[sizeof(pattern) - 1] = 0;
  int resultLength = sprintf(result, pattern, digits);
  if (resultLength > 4 || resultLength < 0) {
    return error;
  }
  return result;
}

String getDigitPattern(int digits, bool colon) {

  if (colon) {
    if (digits < 10) {
      return ":0%d";
    } else {
      return ":%d";
    }
  } else {
    if (digits < 10) {
      return "0%d";
    } else {
      return "%d";
    }
  }
}
