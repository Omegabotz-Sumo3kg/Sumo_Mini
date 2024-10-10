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

// XSHUT SENSORES
#define XSHUT_center 6  
#define XSHUT_left 7
#define XSHUT_right 8

// ENDEREÇO SENSORES
#define ADDRESS_CENTER 0x29  // 0x29
#define ADDRESS_LEFT 0x30     // 0x30
#define ADDRESS_RIGHT 0x31   // 0x31

// PINO IR RECEIVER
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

bool direita = true;
bool desempate = false;
bool opcaoApertado = false;

// Variáveis controle PS4

unsigned long blinkTimer;
bool ledOn = true;
int ledIntensity;

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

IRrecv irrecv(PIN_IRRECV);
decode_results results;

// Protótipos de função
void StartVLs();
void IRRead();
bool readSensor(char pos); // copiado
void MotorWrite(int valorEsquerdo, int valorDireito);
void StatusVerify();
void Auto();

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }
  
  irrecv.enableIRIn();

  PS4.begin("44:1c:a8:c6:41:80");
  while (!PS4.isConnected()) {
    Serial.println("WatingConnection");
    delay(250);
  }
  
  PS4.setLed(100, 0, 0);
  PS4.sendToController();

  StartVLs();
  
  MotorEsquerdo.attach(PIN_MOTORESQ);
  MotorDireito.attach(PIN_MOTORDIR);
  MotorEsquerdo.write(90);
  MotorDireito.write(90);
}

void loop() {
  if (PS4.isConnected()) {
    Status_Verify();
  } else {
    MotorEsquerdo.write(90);
    MotorDireito.write(90);
  }
}

void StartVLs() {
  digitalWrite(XSHUT_CENTER, LOW);
  digitalWrite(XSHUT_LEFT, LOW);
  digitalWrite(XSHUT_RIGHT, LOW);
  delay(10);
  digitalWrite(XSHUT_CENTER, HIGH);
  digitalWrite(XSHUT_LEFT, HIGH);  
  digitalWrite(XSHUT_RIGHT, HIGH);

  if(!s_center.begin()) {
    Serial.println(F("Failed to boot center VL53L0X"));
    while(1);
  }
  delay(10);
  if(!s_left.begin()) {
    Serial.println(F("Failed to boot center VL53L0X"));
    while(1);
  }
  delay(10);
  if(!s_right.begin()) {
    Serial.println(F("Failed to boot center VL53L0X"));
    while(1);
  }
  delay(10);
}

void IRRead(){
  String value;

  if (irrecv.decode(&results)) {
    Serial.println(results.value);
    value = String(results.value, HEX);
    Serial.println(value);
    irrecv.resume();
  }
  if (value == "10") { // Apertou 1 no controle
    PS4.setLed(0, 0, 0);
    PS4.sendToController();
    delay(100);
    PS4.setLed(0, 100, 100);
    PS4.sendToController();

    if (estado == LOCKED) {
      estado = STANDBY;
      PS4.setLed(0, 100, 100);
      PS4.sendToController();
      MotorWrite(90, 90);
    }
  } else if (value == "810") { // Apertou 2 no controle
    if (estado == STANDBY) {
      estado = FIGHTING;
      PS4.setLed(0, 100, 0);
      PS4.sendToController();
    }
  } else if (value == "410") {
    if (estado == FIGHTING || estado == STANDBY) {
      estado = LOCKED;
      PS4.setLed(100, 100, 0);
      PS4.sendToController();
      MotorWrite(90, 90);
    }
  }
}

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

void StatusVerify(){
  if (PS4.Options()){
    if (!opcaoApertado){
      opcaoApertado = !opcaoApertado;
      if (estado == LOCKED){
        estado == STANDBY;
        PS4.setLed(0, 100, 0);
        PS4.sendToController();
        MotorEsquerdo.write(90);
        MotorDireito.write(90);
        Serial.println("STANDBY");
      }
      else if (estado == STANDBY){
        estado == LOCKED;
        PS4.setLed(100, 0, 0);
        PS4.sendToController();
        MotorEsquerdo.write(90);
        MotorDireito.write(90);
        Serial.println("LOCKED");
      }
    }
  } else opcaoApertado = !opcaoApertado;
  Auto();
}

void Auto() {
  IRRead();

  if (estado == STANDBY) {
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

  if (estado == FIGHTING) {
    if (tatic == SUICIDIO) Suicidio();
    else if (tatic == MOVIMENTACAO) Movimentacao();
    else if (tatic == RADAR) Radar();
    else if (tatic == RADAR_INVERSO) RadarInverso();
    else if (tatic == DESVIAR) Desviar();
    else if (tatic == DESVIAR_COM_SENSOR) DesviarComSensor();
    // Verify leds controll
  } else if (estado == STANDBY) {  // Quando estiver no estado de espera, piscar led verde
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
  } else if (estado == LOCKED) {  // Quando estiver no estado para, aumentar e diminuir intensidade do led verde
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

