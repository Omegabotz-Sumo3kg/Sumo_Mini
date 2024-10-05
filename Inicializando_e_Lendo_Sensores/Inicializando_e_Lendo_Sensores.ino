/*==================================
  Ωmegabotz
  Inicalizando 3 Sensores
====================================*/

#include <Wire.h>
#include "Adafruit_VL53L0X.h"

#define XSHUT_center x  // Definir pino
#define XSHUT_left y
#define XSHUT_right z

#define address_center 41  // 0x29
#define address_left 48    // 0x30
#define address_right 49   // 0x31

Adafruit_VL53L0X s_center = Adafruit_VL53L0X();
Adafruit_VL53L0X s_left = Adafruit_VL53L0X();
Adafruit_VL53L0X s_right = Adafruit_VL53L0X();

VL53L0X_RangingMeasurementData_t m_center;  // Measure Central Sensor
VL53L0X_RangingMeasurementData_t m_left;  // Measure Left Sensor
VL53L0X_RangingMeasurementData_t m_right;  // Measure Right Sensor


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
