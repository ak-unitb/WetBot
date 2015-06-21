
Sensor initSensorsByCsv () {

  Bridge.begin();  // Initialize the Bridge (implemtation just step out, if Bridge is already started)  

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

    Serial.print("current free RAM: ");
    Serial.println(getFreeRam());
    delay(100);
    Serial.println("End Loop");
    
    // power on the first active sensor
    if (SENSORs[0].isActive) {
      digitalWrite(SENSORs[0].sensorPinNumber, HIGH);
      return SENSORs[0];
    } else {
      Sensor firstSensor = getNextSensor(SENSORs[0]);
      digitalWrite(firstSensor.sensorPinNumber, HIGH);
      return firstSensor;
    }
    
  } else { // end file exists
    Serial.print("File ");
    Serial.print(filename);
    Serial.println(" doesn't exist!!!!!");
  }

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

// for debugging purposes ... @TODO: remove me, if sketch is working!
void printSensor(Sensor sensor) {
  Serial.print("Sensor{id: ");
  Serial.print(sensor.id);
  Serial.print(", sensorPinNumber: ");
  Serial.print(sensor.sensorPinNumber);
  Serial.print(", relayPinNumber: ");
  Serial.print(sensor.relayPinNumber);
  Serial.print(", frequency: ");
  Serial.print(sensor.frequency);
  Serial.print(", gradeOfDryness: ");
  Serial.print(sensor.gradeOfDryness);
  Serial.print(" (");
  Serial.print(sensor.getGradeOfDrynessLiterally());
  Serial.print(")");
  Serial.print(", previousGradeOfDryness: ");
  Serial.print(sensor.previousGradeOfDryness);
  Serial.print(", frequencyThresholdTooDry: ");
  Serial.print(sensor.frequencyThresholdTooDry);
  Serial.print(", frequencyThresholdTooWet: ");
  Serial.print(sensor.frequencyThresholdTooWet);
  Serial.print(", active: ");
  Serial.print(sensor.isActive);
  Serial.println("};");
}
