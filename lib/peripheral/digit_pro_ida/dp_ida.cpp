#include <Arduino.h>
#include "BLEDevice.h"
#include "bluetooth/configBT.h"
#include "dp_ida.h"

//Variabel Global
static BLEClient *pClient;
static BLERemoteCharacteristic *pRemoteCharacteristic;
static boolean connected2 = false;
static boolean doReconnect2 = false;

// Callback untuk BLE client
class MyClientCallback2 : public BLEClientCallbacks {
  void onConnect(BLEClient *pclient) {
    
  }

  void onDisconnect(BLEClient *pclient) {
    doReconnect2 = true;
  }
};

// Callback untuk Notifikasi
void notifyCallback2(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify) {
  // Serial.print("Data (HEX): ");
  for (size_t i = 0; i < length; i++) {
    Serial.printf("%02X ", pData[i]);
  }
  Serial.println("\n");

  if (pData[19] == 0x31)
  {
    if (length >= 19) {  // Karena kita ingin mengambil byte ke-15, 16, 17, dan 18
      // Ambil byte ke-15 (index 14), ke-16 (index 15), ke-17 (index 16), dan ke-18 (index 17)
      uint8_t byte15 = pData[14];
      uint8_t byte16 = pData[15];
      uint8_t byte17 = pData[16];
      uint8_t byte18 = pData[17];

      // Mengonversi byte menjadi karakter ASCII (char)
      char char15 = (char)byte15;
      char char16 = (char)byte16;
      char char17 = (char)byte17;
      char char18 = (char)byte18;

      // Gabungkan byte ke-15, 16, 17, dan 18 menjadi satu string ASCII
      char result[5];  // Ukuran 5 untuk menyimpan 4 karakter + null terminator
      snprintf(result, sizeof(result), "%c%c%c%c", char15, char16, char17, char18);

      // Konversi string menjadi nilai hex (misalnya, misalkan string "025D" jadi hex 0x025D)
      unsigned long hexValue = strtol(result, NULL, 16);

      // Ubah nilai hex menjadi desimal
      long decimalValue = (long)hexValue;

      // Cetak hasil dalam Kg
      Serial.printf("===== lakukan kirim Data =====\n");
      Serial.printf("Berat Anak: %.2f kg\n", decimalValue / 100.0);
    }
  }else{
    if (length >= 19) {  // Karena kita ingin mengambil byte ke-15, 16, 17, dan 18
      // Ambil byte ke-15 (index 14), ke-16 (index 15), ke-17 (index 16), dan ke-18 (index 17)
      uint8_t byte15 = pData[14];
      uint8_t byte16 = pData[15];
      uint8_t byte17 = pData[16];
      uint8_t byte18 = pData[17];

      // Mengonversi byte menjadi karakter ASCII (char)
      char char15 = (char)byte15;
      char char16 = (char)byte16;
      char char17 = (char)byte17;
      char char18 = (char)byte18;

      // Gabungkan byte ke-15, 16, 17, dan 18 menjadi satu string ASCII
      char result[5];  // Ukuran 5 untuk menyimpan 4 karakter + null terminator
      snprintf(result, sizeof(result), "%c%c%c%c", char15, char16, char17, char18);

      // Cetak hasil string ASCII
      Serial.print("String ASCII dari byte 15-18: ");
      Serial.println(result);

      // Konversi string menjadi nilai hex (misalnya, misalkan string "025D" jadi hex 0x025D)
      unsigned long hexValue = strtol(result, NULL, 16);

      // Cetak hasil dalam format HEX
      Serial.print("Nilai Hexadecimal: ");
      Serial.printf("%lX\n", hexValue);

      // Ubah nilai hex menjadi desimal
      long decimalValue = (long)hexValue;

      // Cetak hasil dalam Kg
      Serial.printf("Berat Anak: %.2f kg\n", decimalValue / 100.0);
    }
  }
  
}

boolean connectToDevice2(){
  if(!pClient){
    BLEDevice::init("");
    pClient = BLEDevice::createClient();
    pClient->setClientCallbacks(new MyClientCallback2());
  }

  BLEAddress deviceAddress2(mac_address2);

  if (!pClient->connect(deviceAddress2)) {
    return false;
  }

  BLEUUID serviceUUID2(serviceUUIDStr2);
  BLERemoteService *pRemoteService2 = pClient->getService(serviceUUID2);
  if (pRemoteService2 == nullptr) {
    pClient->disconnect();
    return false;
  }

  BLEUUID charUUID2(charUUIDStr2);
  pRemoteCharacteristic = pRemoteService2->getCharacteristic(charUUID2);
  if (pRemoteCharacteristic == nullptr) {
    pClient->disconnect();
    return false;
  }

  // Aktifkan notifikasi jika karakteristik mendukung
  if (pRemoteCharacteristic->canNotify()) {
    pRemoteCharacteristic->registerForNotify(notifyCallback2);
  }
  return true;
  
}

void setup_dp_ida() {
    if (connectToDevice2()){
        Serial.println("Perangkat terhubung");
    } else{
        Serial.println("Perangkat tidak terhubung");
        delay(1000);
        setup_dp_ida();
    }
}

void dp_ida_handle() {
if (connected2 && pRemoteCharacteristic){

  } else if (doReconnect2){
    doReconnect2 = false;
    delay(10);
    connectToDevice2();
  } else{
    delay(1000);
  }
}