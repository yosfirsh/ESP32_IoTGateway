#include <Arduino.h>
#include <BLEDevice.h>
#include "bluetooth/configBT.h"
#include "wifi/wifi_mqtt.h"
#include "digit_pro_baby/dp_baby.h"
#include "digit_pro_ida/dp_ida.h"

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  setup_dp_baby();
  setup_dp_ida();
}

void loop() {
  dp_baby_handle();
  dp_ida_handle();
}