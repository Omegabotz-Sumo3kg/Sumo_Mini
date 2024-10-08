/* ==================================
 * Ωmegabotz
 * All code
 * ==================================
 */

// Bibliotecas do sensor
#include "Adafruit_VL53L0X.h"
#include <Wire.h>

// Bibliotecas de servo
#include <ESP32Servo.h>
#include <Arduino.h>

// Biblioteca de infravermelho

#include <IRremote.h>

// Bibliotecas do controle de PS4 

#include <ps4.h>
#include <PS4Controller.h>
#include <ps4_int.h>

/* ================================
 * Definições
 * ================================
 */  
// Definições de pinos dos sensores

#define XSHUT_center x  
#define XSHUT_left y
#define XSHUT_right z

#define address_center 41  // 0x29
#define address_left 48    // 0x30
#define address_right 49   // 0x31

#define PIN_IRRECV 23

#define PIN_MOTORDIR 26
#define PIN_MOTORESQ 27

#define MIN_RANGE 30
#define MAX_RANGE 770

// Enumeradores

enum Estados = {
  LOCKED, STANDBY, FIGHTING
};

enum Estrategias = {
  NONE,
  RADAR,
  SUICIDIO,
  MOVIMENTACAO,
  RADAR_INVERSO,
  DESVIAR,
  DESVIAR_COM_SENSOR
};

enum Teclas = {
  NONE,
  SQUARE,
  CIRCLE,
  TRIANGLE,
  CROSS
}

// Variáveis

Estados estado = LOCKED;
Estrategias estrategia = RADAR;
Teclas tecla = NONE;

/* ===================================
 * Componentes
 * ===================================
 */ 

Adafruit_VL53L0X s_center = Adafruit_VL53L0X();
Adafruit_VL53L0X s_left = Adafruit_VL53L0X();
Adafruit_VL53L0X s_right = Adafruit_VL53L0X();

VL53L0X_RangingMeasurementData_t m_center;  // Measure Central Sensor
VL53L0X_RangingMeasurementData_t m_left;  // Measure Left Sensor
VL53L0X_RangingMeasurementData_t m_right;  // Measure Right Sensor

Servo MotorDireito;
Servo MotorEsquerdo;

// Protótipos de função
void IRRead();
void sensorTest();
bool readSensor(char pos);
void MotorWrite(int valorEsquerdo, int valorDireito);
void statusVerify();
void StartVLs();
void Auto();

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }
  if (!Sensor.begin()) {
    Serial.println(F("Falhou no boot do sensor VL53L0X"));
    while (1)
      ;
  }
  MotorEsquerdo.attach(PIN_MOTORESQ);
  MotorDireito.attach(PIN_MOTORDIR);
  MotorEsquerdo.write(90);
  MotorDireito.write(90);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorRead();
}

// Refacção? de statusVerify?

void IRRead(){
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

void sensorTest(){
  Sensor.rangingTest(&measure, false);
  if (measure.RangeStatus != 4){
    Serial.println("Sensor funcionando")
  }
}

/*void sensorRead(){ 
  Sensor.rangingTest(&measure, false);
  if (measure.RangeMilliMeter > MIN_RANGE && measure.RangeMilliMeter < MAX_RANGE){
    Serial.println("Detectado dentro do range");
    Serial.println(measure.RangeMilliMeter);
  }
  else{
    Serial.println("Fora do range");
  }
  delay(100);
}*/

/*
 * Possível refatoração em readSensor (?) 
  bool readSensor(Adafruit_VL53L0X *sensor, VL53L0X_RangingMeasurementData_t *measure){
    sensor.rangingTest(measure, false);
    if (*measure.RangeMilliMeter > MIN_RANGE && *measure.RangeMilliMeter < MAX_RANGE && *measure.RangeStatus != 4) return 1;
    else return 0;  
  }
*/

/*  Função readSensor retorna 1 quando detectar um objeto.
    Usar dentro de lógicas como:
    > if(readSensor('c')){...} */
bool readSensor(char pos) {
  swtich(pos) {
    case 'c':
      s_center.rangingTest(&m_center, false);
      if (m_center.RangeMilliMeter > MIN_RANGE && m_center.RangeMilliMeter < MAX_RANGE && m_center.RangeStatus != 4) return 1;
      else return 0;
      break;

    case 'l':
      s_left.rangingTest(&m_left, false);
      if (m_left.RangeMilliMeter > MIN_RANGE && m_left.RangeMilliMeter < MAX_RANGE && m_left.RangeStatus != 4) return 1;
      else return 0;
      break;

    case 'r':
      s_right.rangingTest(&m_right, false);
      if (m_right.RangeMilliMeter > MIN_RANGE && m_right.RangeMilliMeter < MAX_RANGE && m_right.RangeStatus != 4) return 1;
      else return 0;
      break;

    default:
      return 0;
      break;
  }
}

void MotorWrite(int valorEsquerdo, int valorDireito){
  MotorDireito.write(valorDireito);
  MotorEsquerdo.write(valorEsquerdo);
}

/* A função startVLs deve estar em void setup()
  logo após de Serial.begin() e Wire.begin() */
void startVLs() {
  pinMode(XSHUT_center, OUTPUT);
  pinMode(XSHUT_left, OUTPUT);
  pinMode(XSHUT_right, OUTPUT);

  pinMode(XSHUT_center, INPUT);
  delay(10);
  s_center.setAddress(address_center);
  if (!s_center.begin(address_center)) {
    Serial.println(F("Failed to boot VL53L0X central"));
    while (1);
  }

  pinMode(XSHUT_left, INPUT);
  delay(10);
  s_left.setAddress(address_left);
  if (!s_left.begin(address_left)) {
    Serial.println(F("Failed to boot VL53L0X left"));
    while (1);
  }

  pinMode(XSHUT_right, INPUT);
  delay(10);
  s_right.setAddress(address_right);
  if (!s_right.begin(address_right)) {
    Serial.println(F("Failed to boot VL53L0X right"));
    while (1);
  }
}

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

