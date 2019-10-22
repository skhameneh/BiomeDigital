#include <Board.h>
#include <Cloud.h>
#include <GattMan.h>
#include <Light.h>
#include <Logger.h>
#include <Settings.h>
#include <StageManager.h>
#include <Sun.h>
#include <WifiManager.h>
#include <SetupModes.h>


uint32_t value = 0;

const int freq = 1000000;
const int ledChannel = 0;
const int resolution = 11;

int dutyMin = 0;
int dutyMax = pow(2, resolution) - 1; //16383; //8191;

static void (*stageLoop)() = NULL;
//static std::function<void()> stageLoop;

void brightnessControlLoop() {
  updateBrightness();
  ledcWrite(ledChannel, int(brightness * dutyMax));
  delay(800);
}


void interactiveStageSetup() {
  LOGGER.println("Connect to server...");
  socketIO.begin("192.168.1.157", 3333);

  socketIO.onEvent(socketIOEvent);

  printLocalTime();

  LOGGER.println("Updating sun cycles...");
  updateSunCycles();

  LOGGER.println("Ready");

  digitalWrite(LED_PIN, LOW);
}

void interactiveStageLoop() {
  updateBrightness();
  ledcWrite(ledChannel, int(brightness * dutyMax));
  
  // Set to less than 1s for high duties,
  // high resolution resolution requires more frequent updates.
  delay(800);

  cloudLoop();
}

void setupStageInit() {
  LOGGER.println("setupStageInit");

  if(mainConfig.wifiConfigured) {
    verifyWifiAndConnect();

    interactiveStageSetup();
    stageLoop = &interactiveStageLoop;
  } else {
    startBLE();
  }
}

void setupStageLoop() {
  LOGGER.println("setupStageLoop");

  if(deviceConnected) {

    if(deviceSetupMode == SETUP_MODE_CONNECT && strlen(wifiConfig.auth) != 0 && strlen(wifiConfig.ssid) != 0) {
      if(wifiMulti.run() == WL_CONNECTED && WiFi.localIP().toString() != "0.0.0.0") {
        LOGGER.println("WiFi connected, stopping BLE.");
        btStop();

        wifiVerified = true;
        settings->updateWifiConfig();

        interactiveStageSetup();
        stageLoop = &interactiveStageLoop;
      } else {
        verifyWifiAndConnect();
      }

      delay(100);
      return;
    }

    if(deviceSetupMode != SETUP_MODE_SCAN) {
      return;
    }

    if(!wifiVerified && !scanStarted) {
      scanStarted = true;

      LOGGER.println("Scan networks...");
      WiFi.scanNetworks(true);
    }

    int n = WiFi.scanComplete();
    if(n >= 0) {
      std::string outString;
      std::string networkList = "";

      LOGGER.print(n);
      LOGGER.println(" found.");

      //sprintf(outString, "%s%d", base_string, number);
      char buf[sizeof(int)*6+2];

      for (int i = 0; i < n; i++) {
        networkList.append(WiFi.SSID(i).c_str());
        networkList.append("\n");
        
        snprintf(buf, sizeof buf, "%d", WiFi.channel(i));
        networkList.append(buf);
        networkList.append("\n");
        
        snprintf(buf, sizeof buf, "%d", WiFi.RSSI(i));
        networkList.append(buf);
        networkList.append("\n");
        
        snprintf(buf, sizeof buf, "%d", WiFi.encryptionType(i));
        networkList.append(buf);
        networkList.append("\n");
      }

      pNetworksCharacteristic->setValue(networkList.data());
      pNetworksCharacteristic->notify();

      WiFi.scanDelete();
      scanStarted = false;
    }

    delay(1000); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
  } else if(oldDeviceConnected) {
    delay(500); // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    LOGGER.println("BLE Advertising");
    oldDeviceConnected = false;
  } else {
    delay(1000);
  }

  // connecting
  if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
    oldDeviceConnected = true;
  }
}

void setup() {
  setupLogger();

  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(FADE_PIN, ledChannel);

  Serial.begin(115200);

  //Settings mySettings;
  settings = new Settings(); //&mySettings;

  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT);

  settings->checkReset();
  
  digitalWrite(LED_PIN, HIGH);

  stageLoop = &setupStageLoop;

  setupStageInit();
}

void loop() {
  stageLoop();
}
