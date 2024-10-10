void Radar() {
  Serial.println("Radar started");
  if (!desempate) {
    unsigned int timerStart = millis() + 300;
    while (timerStart > millis()) {
      MotorWrite(120, 120);
    }
  }
  // Travado não faz nada
  while (estado == LOCKED) {
    IRRead();
    MotorWrite(0, 0);
    Status_Verify();
  }

  while (estado == FIGHTING) {
    // Não achou => Fica procurando
    while ((!readSensor('r') && !readSensor('l') && !readSensor('c')) && estado == FIGHTING) {
      IRRead();
      //Status_Verify();
      if (right) {
        MotorWrite(95, 110);
      } else {
        MotorWrite(110, 95);
      }
    }
    direita = !direita;
    // Achou no meio => Ataca
    while (readSensor('c') && estado == FIGHTING) {
      IRRead();
      MotorWrite(150, 150);
    }
  }
}

void Movimentacao() {
  static int movimento = 0;
  static bool direcao = true;
  int i = 0;
  Serial.println("Movimentacao started");
  if (sensorRead('c') && estado == FIGHTING) {
    MotorWrite(130, 130);
  } else if (sensorRead('l') && estado == FIGHTING) {
    MotorWrite(80, 95);
    delay(100);
  } else if (sensorRead('r') && estado == FIGHTING) {
    MotorWrite(95, 80);
    delay(100);
  }

  if (direcao) direcao = !direcao;
  else if (sensorRead('c') && estado == FIGHTING) {
    do {
      if (movimento < 2) {
        MotorWrite(100, 100);
        direcao = true;
      } else if (movimento < 1) {
        MotorWrite(80, 80);
        direcao = false;
      } else {
        movimento = 0;
      }
      movimento++;
      cont++;
    } while (cont <= 50 || digitalRead(middleInfSensor) == false);
  }
}

void RadarInverso() {
  Serial.println("Radar Inverso started");
  if (!desempate) {
    unsigned int timerStart = millis() + 300;
    while (timerStart > millis()) {
      MotorWrite(120, 120);
    }
  }
  // Travado não faz nada
  while (estado == LOCKED) {
    IRRead();
    MotorWrite(0, 0);
    Status_Verify();
  }

  while (estado == FIGHTING) {
    // Não achou => Fica procurando
    while ((!readSensor('r') && !readSensor('l') && !readSensor('c')) && estado == FIGHTING) {
      IRRead();
      //Status_Verify();
      if (right) {
        MotorWrite(95, 110);
      } else {
        MotorWrite(110, 95);
      }
    }

    while (!readSensor('c')) {

      while (readSensor('l') && estado == FIGHTING) {
        IRRead();
        MotorWrite(115, 130);
      }

      while (readSensor('r') && estado == FIGHTING) {
        //Serial.println("                Right find");
        IRRead();
        MotorWrite(130, 115);
      }
    }

    while (readSensor('c') && estado == FIGHTING) {
      IRRead();
      MotorWrite(150, 150);
    }
    while (estado == LOCKED) {
      IRRead();
      MotorWrite(0, 0);
      Status_Verify();
    }
  }
}

void Suicidio() {
  Serial.println("SuicidioStart");
  if (!desempate) {
    unsigned int timerStart = millis() + 300;
    while (timerStart > millis()) {
      IRRead();
      MotorWrite(120, 120);
    }
  }
  while (estado == FIGHTING){
    IRRead();
    while (sensorRead('c')) {
      IRRead();
      MotorWrite(150, 150);
      PS4.setLed(100, 0, 0);
      Status_Verify();
    }
  }
  while (estado == LOCKED){
    IRRead();
    MotorWrite(0, 0);
    Status_Veri
  }
}