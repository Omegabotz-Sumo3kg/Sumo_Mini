#include <Wire.h>
#include "Adafruit_VL53L0X.h"

#define MIN_RANGE 30
#define MAX_RANGE 770

Adafruit_VL53L0X sensor = Adafruit_VL53L0X();

void sensorRead();

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }
  if (!sensor.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1)
      ;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorRead();
}

void sensorRead(){
  VL53L0X_RangingMeasurementData_t measure;
  sensor.rangingTest(&measure, false);
  if (measure.RangeMilliMeter > MIN_RANGE && measure.RangeMilliMeter < MAX_RANGE && measure.RangeStatus != 4){
    Serial.println("Detectado dentro do range");
    Serial.println(measure.RangeMilliMeter);
  }
  else{
    Serial.println("Fora do range");
  }
  delay(100);
}