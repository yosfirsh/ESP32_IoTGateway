#include <Arduino.h>
#include "wifi/wifi_mqtt.h"
#include "ble_handler/ble_handler.h"

//setting wifi
const char *ssid = "Elitech";   // Ganti dengan nama SSID Wi-Fi Anda
const char *password = "wifis1nko"; // Ganti dengan password Wi-Fi Anda

//setting Service & Characteristic BLE
const char *serviceUUID = "0000ffe0-0000-1000-8000-00805f9b34fb";
const char *charUUID = "0000FFE2-0000-1000-8000-00805F9B34FB";

void setup() {
  Serial.begin(115200);

  // setup_wifi_mqtt(ssid, password);

  setup_ble(serviceUUID, charUUID);

}

void loop() {
  ble_handler();
  
}