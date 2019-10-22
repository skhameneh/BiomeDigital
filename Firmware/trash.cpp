class MyClientCallback : public BLEClientCallbacks {
    void onConnect(BLEClient* pclient) {
      bleConnected = true;
      LOGGER.print("onConnect");
    }

    void onDisconnect(BLEClient* pclient) {
      connected = false;
      bleConnected = false;
      LOGGER.print("onDisconnect");
    }
};

bool connectToServer() {
  LOGGER.print("Forming a connection to ");
  LOGGER.print(myDevice->getAddress().toString().c_str());

  BLEClient*  pClient  = BLEDevice::createClient();
  LOGGER.print(" - Created client");

  pClient->setClientCallbacks(new MyClientCallback());

  // Connect to the remove BLE Server.
  pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
  LOGGER.print(" - Connected to server");

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    LOGGER.print("Failed to find our service UUID: ");
    LOGGER.print(serviceUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  LOGGER.print(" - Found our service");


  // Obtain a reference to the characteristic in the service of the remote BLE server.
  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr) {
    LOGGER.print("Failed to find our characteristic UUID: ");
    LOGGER.print(charUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  LOGGER.print(" - Found our characteristic");

  // Read the value of the characteristic.
  if (pRemoteCharacteristic->canRead()) {
    std::string value = pRemoteCharacteristic->readValue();
    LOGGER.print("The characteristic value was: ");
    LOGGER.print(value.c_str());
  }

  if (pRemoteCharacteristic->canNotify())
    pRemoteCharacteristic->registerForNotify(notifyCallback);

  connected = true;
}

/**
   Scan for BLE servers and find the first one that advertises the service we are looking for.
*/
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    /**
        Called for each advertising BLE server.
    */
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      LOGGER.print("BLE Advertised Device found: ");
      LOGGER.print(advertisedDevice.toString().c_str());

      // We have found a device, let us now see if it contains the service we are looking for.
      if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

        BLEDevice::getScan()->stop();
        myDevice = new BLEAdvertisedDevice(advertisedDevice);
        doConnect = true;
        doScan = true;

      }
    }
};

void doScanFn() {
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  //pBLEScan->start(5, false);
}

void connectOrScanLoop() {
  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are
  // connected we set the connected flag to be true.
  if (doConnect == true) {
    if (connectToServer()) {
      LOGGER.print("Connected to the BLE Server.");
    } else {
      LOGGER.print("Failed to connect to the server.");
    }
    doConnect = false;
  }

  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (connected) {
    String newValue = "Time since boot: " + String(millis() / 1000);
    LOGGER.print("Setting new characteristic value to \"" + newValue + "\"");

    // Set the characteristic's value to be the array of bytes that is actually a string.
    pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
  } else if (doScan) {
    BLEDevice::getScan()->start(0);  // this is just eample to start scan after disconnect, most likely there is better way to do it in arduino
  }
}

void oldChunkOfCode() {
      if (false) {
    if(!scanStarted) {
      scanStarted = true;

      pNetworksCharacteristic->setValue("hello");

      LOGGER.println("Scan networks...");
      int n = WiFi.scanNetworks(true);
    }

    int n = WiFi.scanComplete();
    if(n >= 0) {
      std::string networkList = "";

      Serial.printf("%d network(s) found\n", n);

      for (int i = 0; i < n; i++) {
        //Serial.printf("%d: %s, Ch:%d (%ddBm) %s\n", i+1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : "");
        //      for (int i = 0; i < n; ++i) {
        
        //WiFi.SSID(i).toCharArray(ssidList[i], 64);
        networkList.append(WiFi.SSID(i).c_str());
        networkList.append("\n");
      }

      LOGGER.printf(networkList.data());

      //uint8_t *networksData = (uint8_t *) ssidList;
      pNetworksCharacteristic->setValue(networkList.data());

      WiFi.scanDelete();
      scanStarted = false;
    }

    //uint8_t *networksData = (uint8_t *) ssidList;
    //uint8_t slidePressure = (uint8_t)atoi(bufferSlidePressure);
  }
}