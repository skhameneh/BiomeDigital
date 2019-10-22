#ifndef GATTMAN_H
#define GATTMAN_H

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <Settings.h>
#include <WifiManager.h>
#include <SetupModes.h>

#define SERVICE_UUID                    "bb45a210-56c6-4d35-8840-35e57dc20994"
#define CHARACTERISTIC_UUID_STATUS      "be5fc666-e2e5-4b8a-8ce6-ab7f8dcbdc5a"
#define CHARACTERISTIC_UUID_NETWORKS    "a38bfd96-f6ff-4b5b-858b-80f2abdd1859"
#define CHARACTERISTIC_UUID_SSID        "6c754c96-8410-45de-bb04-e2aee7851393"
#define CHARACTERISTIC_UUID_AUTH        "714fedf0-e6f7-4d7b-b7a4-197b1ae04840"
#define CHARACTERISTIC_UUID_MODE        "f132fb80-32be-4c29-b67f-f78be607eeb4"

// The remote service we wish to connect to.
//static BLEUUID serviceUUID(S_UUID);
// The characteristic of the remote service we are interested in.
//static BLEUUID charUUID("be5fc666-e2e5-4b8a-8ce6-ab7f8dcbdc5a");

//static BLERemoteCharacteristic* pRemoteCharacteristic;
//static BLEAdvertisedDevice* myDevice;

bool deviceConnected = false;
bool oldDeviceConnected = false;

BLEServer *pServer = NULL;
BLECharacteristic *pNetworksCharacteristic = NULL;
BLECharacteristic *pSsidCharacteristic = NULL;
BLECharacteristic *pAuthCharacteristic = NULL;
BLECharacteristic *pModeCharacteristic = NULL;


class SsidCharacteristicCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();

    if (value.length() > 0) {
      LOGGER.println("Got SSID");
      networkSsid = value;

      settings->updateWifiConfig();
    }
  }
};

class AuthCharacteristicCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();

    if (value.length() > 0) {
      LOGGER.println("Got AUTH");
      networkAuth = value;

      settings->updateWifiConfig();
    }
  }
};

class ModeCharacteristicCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();

    if (value.length() > 0) {
      int8_t mode;
      mode = value[0];
      LOGGER.print("Got MODE: ");
      LOGGER.println(mode);
      deviceSetupMode = (int)mode;
    }
  }
};

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      //bleConnected = true;
      deviceConnected = true;
      LOGGER.println("onConnect");
    };

    void onDisconnect(BLEServer* pServer) {
      //bleConnected = false;
      deviceConnected = false;
      LOGGER.println("onDisconnect");
    }
};

void startBLE() {
  BLEDevice::init("TestClient");
  BLEDevice::setMTU(247);


  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  pNetworksCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID_NETWORKS,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_NOTIFY |
    BLECharacteristic::PROPERTY_INDICATE
  );
  pNetworksCharacteristic->addDescriptor(new BLE2902());

  pSsidCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID_SSID,
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_NOTIFY |
    BLECharacteristic::PROPERTY_INDICATE
  );
  pSsidCharacteristic->addDescriptor(new BLE2902());

  pAuthCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID_AUTH,
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_NOTIFY |
    BLECharacteristic::PROPERTY_INDICATE
  );
  pAuthCharacteristic->addDescriptor(new BLE2902());

  pModeCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID_MODE,
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_NOTIFY |
    BLECharacteristic::PROPERTY_INDICATE
  );
  pModeCharacteristic->addDescriptor(new BLE2902());



  pSsidCharacteristic->setCallbacks(new SsidCharacteristicCallbacks());
  pAuthCharacteristic->setCallbacks(new AuthCharacteristicCallbacks());
  pModeCharacteristic->setCallbacks(new ModeCharacteristicCallbacks());
  
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  
  pAdvertising->setMinPreferred(0x0);

  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x06);  
  pAdvertising->setMinPreferred(0x12);

  BLEDevice::startAdvertising();
  LOGGER.println("BLE On");
}

#endif