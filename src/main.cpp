#include <Arduino.h>
#include <BLEDevice.h>
#include "bluetooth/configBT.h"
#include "wifi/wifi_mqtt.h"
#include "digit_pro_baby/dp_baby.h"


void setup() {
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  setup_dp_baby();
}

void loop() {
  dp_baby_handle();
}