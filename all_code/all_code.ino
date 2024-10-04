// Biblioteca do sensor
#include "Adafruit_VL53L0X.h"

// Bibliotecas de servo
#include <ESP32Servo.h>
#include <Arduino.h>

// Biblioteca de infravermelho

#include <IRremote.h>

// Definições 

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
  RADAR, RADAR_INVERSO, SUICIDIO, MOVIMENTACAO
};

// Variáveis

Estados estado = LOCKED;

Estrategias estrategia = RADAR;

// Componentes

Adafruit_VL53L0X Sensor = Adafruit_VL53L0X();
VL53L0X_RangingMeasurementData_t measure;
Servo MotorDireito;
Servo MotorEsquerdo;

// Protótipos de função

void statusVerify();
void sensorRead();

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

void sensorTest(){
  Sensor.rangingTest(&measure, false);
  if (measure.RangeStatus != 4){
    Serial.println("Sensor funcionando")
  }
}

void sensorRead(){ 
  Sensor.rangingTest(&measure, false);
  if (measure.RangeMilliMeter > MIN_RANGE && measure.RangeMilliMeter < MAX_RANGE){
    Serial.println("Detectado dentro do range");
    Serial.println(measure.RangeMilliMeter);
  }
  else{
    Serial.println("Fora do range");
  }
  delay(100);
}