
void initSensorsByCsv () {


  // Setup File IO
  FileSystem.begin();

  if (FileSystem.exists(filename)) {

    File csv = FileSystem.open(filename, FILE_READ);
    
    int id = 0;
    uint32_t frequencyThresholdTooWet = 0;
    uint32_t frequencyThresholdTooDry = 0;
    bool isActive = false;
    
    String csvString = "";
    long csvValue = 0;
    int lineCounter = 0;
    int partsCounter = 0;
    double valueCounter = 0.0;
    while (csv.available()) {
      char c = csv.read();
      if (c == ',' || c == '\n') {
        
        // do something with csvValue...
        /* * /
        Serial.println();
        Serial.print("Value for line: ");
        Serial.print(lineCounter);
        Serial.print(" part: ");
        Serial.print(partsCounter);
        Serial.print(" => value: ");
        //Serial.println(csvValue);
        Serial.println(csvString);
        
        Serial.print("to int: ");
        /* */
        str2int(csvValue, csvString.c_str(), 10);
        //Serial.println(csvValue);
        
        if (partsCounter == 0) {
          id = (int)csvValue;
        } else if (partsCounter == 1) {
          frequencyThresholdTooWet = (uint32_t)csvValue;
        } else if (partsCounter == 2) {
          frequencyThresholdTooDry = (uint32_t)csvValue;
        } else if (partsCounter == 3) {
          isActive = (csvValue == 1) ? true : false;
        }
        
        //csvValue = 0;
        csvString = "";
        valueCounter = 0;
  
        if (c == ',') {
          partsCounter++;
        } else { // need to be linefoot
          Sensor sensor ((id - 1), (uint32_t)(frequencyThresholdTooWet), (uint32_t)(frequencyThresholdTooDry), isActive);
          SENSORs[lineCounter] = sensor;
          printSensor(SENSORs[lineCounter]);
          lineCounter++;
          partsCounter = 0;
  
          /* * /
          Serial.println();
          Serial.print("current free RAM: ");
          Serial.println(getFreeRam());
          Serial.println();
          /* */
          
          delay(100);
        }
  
      } else {
        //Serial.print(c);
        //csvValue += ((uint32_t)c - 48) * pow(10.0, valueCounter);
        csvString += c;
        valueCounter += 1;
      }
  
    } // end while
    csv.close();
  } else { // end file exists
    Serial.print("File ");
    Serial.print(filename);
    Serial.println(" doesn't exist!!!!!");
  }

  Serial.print("current free RAM: ");
  Serial.println(getFreeRam());
  delay(100);
  Serial.println("End Loop");
  
  Serial.println("done... ");

}

STR2INT_ERROR str2int (long &i, char const *s, int base = 10) {
    char *end;
    long  l;
    int errno = 0;
    l = strtol(s, &end, base);
    /* <range.h> or similar is missong ... :{
    if ((errno == ERANGE && l == LONG_MAX) || l > INT_MAX) {
        return OVERFLOW;
    }
    if ((errno == ERANGE && l == LONG_MIN) || l < INT_MIN) {
        return UNDERFLOW;
    }
    */
    if (*s == '\0' || *end != '\0') {
        return INCONVERTIBLE;
    }
    i = l;
    return SUCCESS;
}


int getFreeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
