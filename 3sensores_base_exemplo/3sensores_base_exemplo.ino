#include "Adafruit_VL53L0X.h"

#define CENTER_ADDRESS 0x30
#define LEFT_ADDRESS 0x31
#define RIGHT_ADDRESS 0x32

#define XSHUT_CENTER 6
#define XSHUT_LEFT 7
#define XSHUT_RIGHT 8

Adafruit_VL53L0X s_center = Adafruit_VL53L0X();
Adafruit_VL53L0X s_left = Adafruit_VL53L0X();
Adafruit_VL53L0X s_right = Adafruit_VL53L0X();

VL53L0X_RangingMeasurementData_t m_center;  // Measure Central Sensor
VL53L0X_RangingMeasurementData_t m_left;  // Measure Left Sensor
VL53L0X_RangingMeasurementData_t m_right;  // Measure Right Sensor

void setupVLs();
void readAllSensors();

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

void setupVLs(){
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

void readAllSensors(){
  s_center.rangingTest(&m_center, false);
  s_left.rangingTest(&m_left, false);
  s_right.rangingTest(&m_right, false);

  Serial.print(F("Center: "));
  if(m_center.RangeStatus != 4) {     // if not out of range
    Serial.print(m_center.RangeMilliMeter);
  } else {
    Serial.print(F("Out of range"));
  }
  Serial.print(F(" "));
  Serial.print(F("Left: "));
  if(m_left.RangeStatus != 4) {     // if not out of range
    Serial.print(m_left.RangeMilliMeter);
  } else {
    Serial.print(F("Out of range"));
  }
  Serial.print(F(" "));
  Serial.print(F("Right: "));
  if(m_right.RangeStatus != 4) {     // if not out of range
    Serial.print(m_right.RangeMilliMeter);
  } else {
    Serial.print(F("Out of range"));
  }
}
