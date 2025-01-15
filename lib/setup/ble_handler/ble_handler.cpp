#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEClient.h>
#include "ble_handler/ble_handler.h"

static BLEUUID serviceUUID;
static BLEUUID charUUID;

//BLE Server
static boolean connected = false;
static boolean doConnect = false;
static boolean doScan = false;
static BLEAdvertisedDevice *myDevice;
static BLERemoteCharacteristic *pRemoteCharacteristic;

static void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify) {
  // Serial.print("Notify callback for characteristic ");
  // Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
  // Serial.print(" of data length ");
  // Serial.println(length);
  // Serial.print("Berat: ");
  // Serial.write(pData, length);
  // Serial.println();

  Serial.print("Data (HEX): ");
    for (size_t i = 0; i < length; i++) {
        Serial.printf("%02X ", pData[i]); // Cetak byte sebagai 2-digit hex
    }
    Serial.println();

    // // Jika data memiliki nilai tertentu, misalnya berat
    // if (length >= 2) { // Misalnya, nilai berat dikodekan dalam 2 byte pertama
    //     uint16_t berat = (pData[0] << 8) | pData[1]; // Gabungkan dua byte menjadi uint16_t
    //     Serial.print("Berat (dalam gram): ");
    //     Serial.println(berat);
    // }
}

//Mencari device
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
   Serial.print("BLE Advertised Device found: ");
   Serial.println(advertisedDevice.toString().c_str());
   // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;
    }
  }
};

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient *pclient) {}

  void onDisconnect(BLEClient *pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};

boolean connectToServer() {
  Serial.print("Forming a connection to ");
  Serial.println(myDevice->getAddress().toString().c_str());
  BLEClient *pClient = BLEDevice::createClient();
  Serial.println(" - Created client");

  pClient->setClientCallbacks(new MyClientCallback());

  // Connect to the remove BLE Server.
  pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
  Serial.println(" - Connected to server");
  pClient->setMTU(517);  //set client to request maximum MTU from server (default is 23 otherwise)

  BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found our service");

  // Obtain a reference to the characteristic in the service of the remote BLE server.
  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(charUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found our characteristic");

    // Read the value of the characteristic.
  if (pRemoteCharacteristic->canRead()) {
    std::string value = pRemoteCharacteristic->readValue();
    Serial.print("The characteristic value was: ");
    Serial.println(value.c_str());
  }

   if (pRemoteCharacteristic->canNotify()) {
    pRemoteCharacteristic->registerForNotify(notifyCallback);
  }

  connected = true;
  return true;

}

void setup_ble(const char *serviceUUIDStr, const char *charUUIDStr) {
  serviceUUID = BLEUUID(serviceUUIDStr);
  charUUID = BLEUUID(charUUIDStr);

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

void ble_handler(){
    
  // Mekalukan connect ke BLE server/ perangkat peripheral
  if (doConnect == true)
  {
    if(connectToServer()){
      Serial.println("We are now connected to the BLE Server.");
    }else{
      Serial.println("We have failed to connect to the server, there is nothin more we will do.");
    }
    doConnect = false;
  }

  if (connected) {
    String newValue = "Time since boot: " + String(millis() / 1000);
    Serial.println("Setting new characteristic value to \"" + newValue + "\"");

    // Set the characteristic's value to be the array of bytes that is actually a string.
    pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
  } else if (doScan) {
    BLEDevice::getScan()->start(0);  // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
  }

  delay(1000);
}