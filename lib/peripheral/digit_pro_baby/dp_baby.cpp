#include <Arduino.h>
#include "BLEDevice.h"
#include "bluetooth/configBT.h"
#include "dp_baby.h"

//Variabel Global
static BLEClient *pClient;
static BLERemoteCharacteristic *pRemoteCharacteristic;
static boolean connected = false;
static boolean doReconnect = false;

// Callback untuk BLE client
class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient *pclient) {
    
  }

  void onDisconnect(BLEClient *pclient) {
    doReconnect = true;
  }
};

// Callback untuk Notifikasi
void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify) {
  // Serial.print("Data (HEX): ");
  for (size_t i = 0; i < length; i++) {
    Serial.printf("%02X ", pData[i]);
  }
  Serial.println();

  if (length >= 7) { // Pastikan ada minimal 7 byte

    uint16_t rawWeight = (pData[5] << 8) | pData[6]; // Byte ke-6 (MSB) dan ke-5 (LSB)
    Serial.printf("Hex Berat : 0x%04X\n", rawWeight);

    // Dekripsi dengan kunci (EE, CF)
    uint16_t key = 0xEECF;
    uint16_t decryptedWeight = rawWeight ^ key; // XOR dengan kunci
    Serial.printf("Hasil Decrypt : 0x%04X\n", decryptedWeight);

    // Konversi ke desimal dan cetak hasil Kg
    float weightInKg = decryptedWeight / 1000.0; //  berat dalam gram dikonversi ke kg
    Serial.printf("Berat Kg : %.3f kg\n", weightInKg);
  } 
}

boolean connectToDevice(){
  if(!pClient){
    BLEDevice::init("");
    pClient = BLEDevice::createClient();
    pClient->setClientCallbacks(new MyClientCallback());
  }

  BLEAddress deviceAddress(mac_address);

  if (!pClient->connect(deviceAddress)) {
    return false;
  }

  BLEUUID serviceUUID(serviceUUIDStr);
  BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    pClient->disconnect();
    return false;
  }

  BLEUUID charUUID(charUUIDStr);
  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr) {
    pClient->disconnect();
    return false;
  }

  // Aktifkan notifikasi jika karakteristik mendukung
  if (pRemoteCharacteristic->canNotify()) {
    pRemoteCharacteristic->registerForNotify(notifyCallback);
  }
  return true;
  
}

void setup_dp_baby() {
 if (connectToDevice()){
    Serial.println("Perangkat terhubung");
  } else{
    Serial.println("Perangkat tidak terhubung");
  }
}

void dp_baby_handle() {
if (connected && pRemoteCharacteristic){

  } else if (doReconnect){
    doReconnect = false;
    delay(10);
    connectToDevice();
  } else{
    delay(1000);
  }
}