void IRRead() {
  String value;

  if (irrecv.decode(&results)) {
    Serial.println(results.value);
    value = String(results.value, HEX);
    Serial.println(value);
    irrecv.resume();
  }
  if (value == "10") { // Apertou 1 no controle
    //if (value == "Ps4.up") {
    PS4.setLed(0, 0, 0);
    PS4.sendToController();
    delay(100);
    PS4.setLed(0, 100, 100);
    PS4.sendToController();

    if (autoState == STOPPED) {
      //Serial.println("ReadyToGo");
      autoState = READY;
      PS4.setLed(0, 100, 100);
      PS4.sendToController();
      MotorWrite(90, 90);
      //CalibrateSensors();
    }
  } else if (value == "810") { // Apertou 2 no controle
    //} else if (value == "Ps4.left") {
    if (autoState == READY) {
      autoState = RUNNING;
      PS4.setLed(0, 100, 0);
      PS4.sendToController();
    }
  } else if (value == "410") {
    //} else if ( value == "Ps4.right") {
    if (autoState == RUNNING || autoState == READY) {
      //Serial.println("STOP");
      autoState = STOPPED;
      PS4.setLed(100, 100, 0);
      PS4.sendToController();
      MotorWrite(90, 90);
    }
  }
}