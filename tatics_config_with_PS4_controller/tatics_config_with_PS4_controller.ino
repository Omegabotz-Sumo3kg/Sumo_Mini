/*==================================
  Ωmegabotz
  Configurando tática pelo PS4 
====================================*/

int combo;
bool bt_press_f;
unsigned long int t_press, t_press_fixo = 1000;

enum tatics {
  NONE,
  RADAR,
  SUICIDIO,
  MOVIMENTACAO,
  RADAR_INVERSO,
  DESVIAR,
  DESVIAR_COM_SENSOR
  //TESTE
};
tatics tatic = RADAR;

enum teclas {
  NONE,
  SQUARE,
  CIRCLE,
  TRIANGLE,
  CROSS
};
teclas tecla = NONE;


void Status_Verify() {
  if (PS4.Options()) {
    if (!optionPressed) {
      optionPressed = true;
      if (robotState == LOCKED) {
        robotState = AUTO;
        PS4.setLed(0, 100, 0);
        PS4.sendToController();
        MotorEsquerdo.write(90);
        MotorDireito.write(90);
        Serial.println("AUTO");

      } else if (robotState == AUTO) {
        robotState = MANUAL;
        PS4.setLed(0, 0, 100);
        PS4.sendToController();
        MotorEsquerdo.write(90);
        MotorDireito.write(90);
        autoState = STOPPED;
        Serial.println("MANUAL");

      } else if (robotState == MANUAL) {
        robotState = LOCKED;
        autoState = STOPPED;
        PS4.setLed(100, 0, 0);
        PS4.sendToController();
        MotorEsquerdo.write(90);
        MotorDireito.write(90);
        Serial.println("LOCKED");
      }
    }
  } else optionPressed = false;

  if (robotState == MANUAL) ManualControl();
  else if (robotState == AUTO) Auto();
}

void Auto() {
  IRRead();

  if (autoState == READY) {
    if (PS4.Square()) {
      t_press = millis();
      bt_press_f = true;
      tecla = SQUARE;
      combo += tecla;
    }
    if (PS4.Circle()) {
      t_press = millis();
      bt_press_f = true;
      tecla = CIRCLE;
      combo += tecla;
    }
    if (PS4.Triangle()) {
      t_press = millis();
      bt_press_f = true;
      tecla = TRIANGLE;
      combo += tecla;
    }
    if (PS4.Cross()) {
      t_press = millis();
      bt_press_f = true;
      tecla = CROSS;
      combo += tecla;
    }

    if (millis() - t_press >= t_press_fixo && bt_press_f) {
      bt_press_f = false;
      combo_salvo = combo;
      combo = 0;
    }

    if (!bt_press_f) {
      if (combo_salvo == RADAR) tatic = RADAR;
      else if (combo_salvo == SUICIDIO) tatic = SUICIDIO;
      else if (combo_salvo == MOVIMENTACAO) tatic = MOVIMENTACAO;
      else if (combo_salvo == RADAR_INVERSO) tatic = RADAR_INVERSO;
      else if (combo_salvo == DESVIAR) tatic = DESVIAR;
      else if (combo_salvo == DESVIAR_COM_SENSOR) tatic = DESVIAR_COM_SENSOR;
      else println("Combo não reconhecido");
    }

    if (PS4.Right()) {
      Serial.println("Right");
      right = true;
    }
    if (PS4.Left()) {
      Serial.println("Left");
      right = false;
    }
    if (PS4.Up()) {
      Serial.println("Up");
      desempate = false;
    }
    if (PS4.Down()) {
      Serial.println("Down");
      desempate = true;
    }
  }

  if (autoState == RUNNING) {
    if (tatic == SUICIDIO) Suicidio();
    else if (tatic == MOVIMENTACAO) Movimentacao();
    else if (tatic == RADAR) Radar();
    else if (tatic == RADAR_INVERSO) RadarInverso();
    else if (tatic == DESVIAR) Desviar();
    else if (tatic == DESVIAR_COM_SENSOR) DesviarComSensor();
    // Verify leds controll
  } else if (autoState == READY) {  // Quando estiver no estado de espera, piscar led verde
    MotorWrite(90, 90);
    if (blinkTimer < millis()) {
      if (ledOn) {
        PS4.setLed(0, 0, 0);
        PS4.sendToController();
      } else {
        PS4.setLed(0, 100, 0);
        PS4.sendToController();
      }
      ledOn = !ledOn;
      blinkTimer = millis() + 200;
    }
  } else if (autoState == STOPPED) {  // Quando estiver no estado para, aumentar e diminuir intensidade do led verde
    MotorWrite(90, 90);
    if (blinkTimer < millis()) {
      if (ledOn) {
        PS4.setLed(0, ledIntensity++, 0);
        PS4.sendToController();
      } else {
        PS4.setLed(0, ledIntensity--, 0);
        PS4.sendToController();
      }
      if (ledIntensity == 0 || ledIntensity == 100) ledOn = !ledOn;
      blinkTimer = millis() + 10;
    }
  }
}