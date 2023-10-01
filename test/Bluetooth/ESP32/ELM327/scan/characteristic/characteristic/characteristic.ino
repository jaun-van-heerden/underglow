#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

static BLEUUID serviceUUID("0000fff0-0000-1000-8000-00805f9b34fb");  // Service UUID to look for

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
      BLEClient* pClient  = BLEDevice::createClient();
      pClient->connect(&advertisedDevice);
      BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
      if (pRemoteService == nullptr) {
        Serial.print("Failed to find our service UUID: ");
        Serial.println(serviceUUID.toString().c_str());
        return;
      }

      // Obtain a reference to the characteristics in the service of the remote BLE server.
      std::map<std::string, BLERemoteCharacteristic*>* pCharacteristicsMap = pRemoteService->getCharacteristics();
      Serial.println("Characteristics found in service:");
      for (auto it = pCharacteristicsMap->begin(); it != pCharacteristicsMap->end(); ++it) {
        BLERemoteCharacteristic* pRemoteCharacteristic = it->second;
        Serial.print("- UUID: ");
        Serial.println(pRemoteCharacteristic->getUUID().toString().c_str());
      }
      
      pClient->disconnect();
    }
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");
  
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000); // Delay for demo purposes
}
