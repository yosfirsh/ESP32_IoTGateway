#include <WiFi.h>
#include <MQTT.h>
#include <config.h>
#include "wifi/wifi_mqtt.h"

WiFiClient net;
MQTTClient client;

void connect() {
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  client.setWill("ESP32TEST_Elitech/status", "offline", true, 1);
  while (!client.connect("12345678", "yosfirsh", "yosfirsh"))
  {
    delay(500);
  }
  client.publish("ESP32TEST_Elitech/status", "online", true, 1);
  client.subscribe("ESP32TEST_Elitech/#", 1); // Subscribe menggunakan QoS 1
}

void setup_wifi_mqtt() {
  // Menghubungkan ke Wi-Fi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(HOSTNAME_WIFI, PASS_WIFI);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Menghubungkan ke MQTT Broker");
  client.begin("149.129.248.169", net);
  client.setOptions(100, true, 1000);
  Serial.println("Terhubung ke broker");

  connect();
}

void handlemqtt(){
  client.loop();
}