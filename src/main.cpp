#include <Arduino.h>
#include <BLEDevice.h>
#include <WiFi.h>
#include <MQTT.h>

const char *ssid = "Elitech";   // Ganti dengan nama SSID Wi-Fi Anda
const char *password = "wifis1nko"; // Ganti dengan password Wi-Fi Anda

WiFiClient net;
MQTTClient client;

static BLEUUID serviceUUID1("370acc92-9c96-4664-98bc-52ca22a19984");
static BLEUUID serviceUUID2("43f2e64b-1a47-4f82-9404-256bb1451fe1");
static BLEUUID charUUID1("4fb93a90-6721-4b0e-86cd-866b2b36af24");
static BLEUUID charUUID2("e68ff4e7-dad5-4871-b9ef-28c8c495c5ab");

//BLE Server 1
static boolean connected1 = false;
static boolean doConnect1 = false;
static boolean doScan1 = false;
static BLEAdvertisedDevice *myDevice1;
static BLERemoteCharacteristic *pRemoteCharacteristic1;

//BLE Server 2
static boolean connected2 = false;
static boolean doConnect2 = false;
static boolean doScan2 = false;
static BLEAdvertisedDevice *myDevice2;
static BLERemoteCharacteristic *pRemoteCharacteristic2;

void connect()
{
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

static void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify) {
  Serial.print("Notify callback for characteristic ");
  Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
  Serial.print(" of data length ");
  Serial.println(length);
  Serial.print("data: ");
  Serial.write(pData, length);
  Serial.println();
}

//Mencari device
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
   Serial.print("BLE Advertised Device found: ");
   Serial.println(advertisedDevice.toString().c_str());
   // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID1)) {
      BLEDevice::getScan()->stop();
      myDevice1 = new BLEAdvertisedDevice(advertisedDevice);
      doConnect1 = true;
      doScan1 = true;
    }
  }
};

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient *pclient) {}

  void onDisconnect(BLEClient *pclient) {
    connected1 = false;
    Serial.println("onDisconnect");
  }
};

//menghubungkan device
boolean connectToServer1() {
  Serial.print("Forming a connection to ");
  Serial.println(myDevice1->getAddress().toString().c_str());
  BLEClient *pClient1 = BLEDevice::createClient();
  Serial.println(" - Created client1");

  pClient1->setClientCallbacks(new MyClientCallback());

  // Connect to the remove BLE Server.
  pClient1->connect(myDevice1);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
  Serial.println(" - Connected to server");
  pClient1->setMTU(517);  //set client to request maximum MTU from server (default is 23 otherwise)

  BLERemoteService *pRemoteService1 = pClient1->getService(serviceUUID1);
  if (pRemoteService1 == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID1.toString().c_str());
    pClient1->disconnect();
    return false;
  }
  Serial.println(" - Found our service");

  // Obtain a reference to the characteristic in the service of the remote BLE server.
  pRemoteCharacteristic1 = pRemoteService1->getCharacteristic(charUUID1);
  if (pRemoteCharacteristic1 == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(charUUID1.toString().c_str());
    pClient1->disconnect();
    return false;
  }
  Serial.println(" - Found our characteristic");

    // Read the value of the characteristic.
  if (pRemoteCharacteristic1->canRead()) {
    std::string value = pRemoteCharacteristic1->readValue();
    Serial.print("The characteristic value was: ");
    Serial.println(value.c_str());
  }

   if (pRemoteCharacteristic1->canNotify()) {
    pRemoteCharacteristic1->registerForNotify(notifyCallback);
  }

  connected1 = true;
  return true;

}
boolean connectToServer2() {
  // Serial.print("Forming a connection to ");
  // Serial.println(myDevice2->getAddress().toString().c_str());
  // BLEClient *pClient = BLEDevice::createClient();
  // Serial.println(" - Created client2");

  // pClient->setClientCallbacks(new MyClientCallback());

  // // Connect to the remove BLE Server.
  // pClient->connect(myDevice2);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
  // Serial.println(" - Connected to server");
  // pClient->setMTU(517);  //set client to request maximum MTU from server (default is 23 otherwise)

  // BLERemoteService *pRemoteService = pClient->getService(serviceUUID1);
  // if (pRemoteService == nullptr) {
  //   Serial.print("Failed to find our service UUID: ");
  //   Serial.println(serviceUUID1.toString().c_str());
  //   pClient->disconnect();
  //   return false;
  // }
  // Serial.println(" - Found our service");

  // // Obtain a reference to the characteristic in the service of the remote BLE server.
  // pRemoteCharacteristic2 = pRemoteService->getCharacteristic(charUUID1);
  // if (pRemoteCharacteristic2 == nullptr) {
  //   Serial.print("Failed to find our characteristic UUID: ");
  //   Serial.println(charUUID1.toString().c_str());
  //   pClient->disconnect();
  //   return false;
  // }
  // Serial.println(" - Found our characteristic");

  //   // Read the value of the characteristic.
  // if (pRemoteCharacteristic2->canRead()) {
  //   std::string value = pRemoteCharacteristic2->readValue();
  //   Serial.print("The characteristic value was: ");
  //   Serial.println(value.c_str());
  // }

  //  if (pRemoteCharacteristic2->canNotify()) {
  //   pRemoteCharacteristic2->registerForNotify(notifyCallback);
  // }

  // connected = true;
  // return true;

}

void setup() {
  Serial.begin(115200);

  // Menghubungkan ke Wi-Fi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

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

  //Inisialisasi BLE Device
  BLEDevice::init("ESP32_Client");

  //Inisialisasi BLE Scan
  BLEScan *pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);

}

void loop() {

  // Mekalukan connect ke BLE server/ perangkat peripheral
  if (doConnect1 == true)
  {
    if(connectToServer1()){
      Serial.println("We are now connected to the BLE Server.");
    }else{
      Serial.println("We have failed to connect to the server, there is nothin more we will do.");
    }
    doConnect1 = false;
  }

  if (connected1) {
    String newValue = "Time since boot: " + String(millis() / 1000);
    Serial.println("Setting new characteristic value to \"" + newValue + "\"");

    // Set the characteristic's value to be the array of bytes that is actually a string.
    pRemoteCharacteristic1->writeValue(newValue.c_str(), newValue.length());
  } else if (doScan1) {
    BLEDevice::getScan()->start(0);  // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
  }

  delay(1000);
  
}