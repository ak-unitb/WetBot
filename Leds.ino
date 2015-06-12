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
    delay(500);
  }
}
