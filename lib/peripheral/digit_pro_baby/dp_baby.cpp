#include <Arduino.h>
#include "BLEDevice.h"
#include "bluetooth/configBT.h"
#include "dp_baby.h"

//Variabel Global
static BLEClient *pClient;
static BLERemoteCharacteristic *pRemoteCharacteristic;
static boolean connected1 = false;
static boolean doReconnect1 = false;

// Callback untuk BLE client
class MyClientCallback1 : public BLEClientCallbacks {
  void onConnect(BLEClient *pclient) {
    
  }

  void onDisconnect(BLEClient *pclient) {
    doReconnect1 = true;
  }
};

// Callback untuk Notifikasi
void notifyCallback1(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify) {
  Serial.print("Data (HEX): ");
  for (size_t i = 0; i < length; i++) {
    Serial.printf("%02X", pData[i]);
  }
  Serial.println("\n");

    // Periksa apakah elemen pertama adalah 0x01
    if (pData[4] == 0x02) {
      Serial.println("===== lakukan kirim data=====");

      uint16_t rawWeight = (pData[5] << 8) | pData[6]; // Byte ke-6 (MSB) dan ke-5 (LSB)

      // Dekripsi dengan kunci (EE, CF)
      uint16_t key = 0xEECF;
      uint16_t decryptedWeight = rawWeight ^ key; // XOR dengan kunci

      // Konversi ke desimal dan cetak hasil Kg
      float weightInKg = decryptedWeight / 1000.0; //  berat dalam gram dikonversi ke kg
      Serial.printf("Berat Kg : %.3f kg\n", weightInKg);
    }else{
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

boolean connectToDevice1(){
  if(!pClient){
    BLEDevice::init("");
    pClient = BLEDevice::createClient();
    pClient->setClientCallbacks(new MyClientCallback1());
  }

  BLEAddress deviceAddress1(mac_address1);

  if (!pClient->connect(deviceAddress1)) {
    return false;
  }

  BLEUUID serviceUUID1(serviceUUIDStr1);
  BLERemoteService *pRemoteService1 = pClient->getService(serviceUUID1);
  if (pRemoteService1 == nullptr) {
    pClient->disconnect();
    return false;
  }

  BLEUUID charUUID1(charUUIDStr1);
  pRemoteCharacteristic = pRemoteService1->getCharacteristic(charUUID1);
  if (pRemoteCharacteristic == nullptr) {
    pClient->disconnect();
    return false;
  }

  // Aktifkan notifikasi jika karakteristik mendukung
  if (pRemoteCharacteristic->canNotify()) {
    pRemoteCharacteristic->registerForNotify(notifyCallback1);
  }
  return true;
  
}

void setup_dp_baby() {
    if (connectToDevice1()){
        Serial.println("Perangkat terhubung");
    } else{
        Serial.println("Perangkat tidak terhubung");
        delay(1000);
        setup_dp_baby();
    }
}

void dp_baby_handle() {
if (connected1 && pRemoteCharacteristic){

  } else if (doReconnect1){
    doReconnect1 = false;
    delay(10);
    connectToDevice1();
  } else{
    delay(1000);
  }
}