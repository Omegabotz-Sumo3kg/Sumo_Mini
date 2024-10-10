/*
  Ωmegabotz

  Teste de Range Sensor VL53L0X V2

  Info:
    Range min.: 30mm
    Range máx.: 1250mm

  Default I2C Address: 0x29
  Necessário alteração do endereço para utilização de mais de um sensor!
  Ref. Alterar Endereço:
  > https://www.youtube.com/watch?v=RRQASevYK3g
  > https://learn.adafruit.com/adafruit-vl53l0x-micro-lidar-distance-sensor-breakout/arduino-code
*/

#include <Wire.h>
#include "Adafruit_VL53L0X.h"

#define MIN_RANGE 30  // Leitura Min.       [mm]
#define MAX_RANGE 770 // Diâmetro do Dojô   [mm]

Adafruit_VL53L0X sensor = Adafruit_VL53L0X();

void sensorRead();

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }
  if (!sensor.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1);
  }
}

void loop() {
  sensorRead();
}

void sensorRead(){
  VL53L0X_RangingMeasurementData_t measure;
  sensor.rangingTest(&measure, false);
  if (measure.RangeMilliMeter > MIN_RANGE && measure.RangeMilliMeter < MAX_RANGE && measure.RangeStatus != 4){
    Serial.println("Detectado dentro do range");
    Serial.println(measure.RangeMilliMeter);
  }
  else Serial.println("Fora do range");
  
  delay(100);
}