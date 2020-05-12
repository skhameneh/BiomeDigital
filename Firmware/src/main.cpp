#define CONFIG_ESP32_WIFI_DEBUG_LOG_ENABLE 1
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


#include <math.h> 
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"



uint32_t value = 0;

const int freq = 1000000;
const int ledChannel = 0;
const int resolution = 11;

int dutyMin = 0;
int dutyMax = pow(2, resolution) - 1; //16383; //8191;

static void (*stageLoop)() = NULL;
//static std::function<void()> stageLoop;

void brightnessControlLoop()
{
  updateBrightness();
  //ledcWrite(ledChannel, int(brightness * dutyMax));
  delay(800);
}

void interactiveStageSetup()
{
  LOGGER.println("Connect to server...");
  //socketIO.begin("192.168.1.157", 3333);

  //socketIO.onEvent(socketIOEvent);

  printLocalTime();

  LOGGER.println("Updating sun cycles...");
  updateSunCycles();

  LOGGER.println("Ready");

  //digitalWrite(LED_PIN, LOW);
}

void interactiveStageLoop()
{
  updateBrightness();
  //ledcWrite(ledChannel, int(brightness * dutyMax));

  // Set to less than 1s for high duties,
  // high resolution resolution requires more frequent updates.
  delay(800);

  cloudLoop();
}

void setupStageInit()
{
  esp_log_level_set("wifi", ESP_LOG_WARN);
  esp_log_level_set("dhcpc", ESP_LOG_INFO);

  LOGGER.println("setupStageInit");

  if (mainConfig.wifiConfigured)
  {
    verifyWifiAndConnect();

    interactiveStageSetup();
    stageLoop = &interactiveStageLoop;
  }
  else
  {
    startBLE();
  }
}

void connectStageLoop()
{
  LOGGER.println("connectStageLoop");
  delay(500);

  //WiFi.status();

  /*
  if (wifiVerified)
  {
    settings->updateWifiConfig();

    interactiveStageSetup();
    stageLoop = &interactiveStageLoop;
  }
  */
}

void setupStageLoop()
{
  LOGGER.println("setupStageLoop");

  if (deviceConnected)
  {

    if (deviceSetupMode == SETUP_MODE_CONNECT && strlen(wifiConfig.auth) != 0 && strlen(wifiConfig.ssid) != 0)
    {
      /*
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
      */

      verifyWifiAndConnect();
      stageLoop = &connectStageLoop;

      //delay(100);
      return;
    }

    if (deviceSetupMode != SETUP_MODE_SCAN)
    {
      return;
    }

    if (!wifiVerified && !scanStarted)
    {
      scanStarted = true;

      LOGGER.println("Scan networks...");
      //WiFi.scanNetworks(true);
    }

    //int n = WiFi.scanComplete();
    int n = 0;

    if (n >= 0)
    {
      //std::string outString;
      //std::string networkList = "";

      //LOGGER.print(n);
      LOGGER.println(" found.");

      //sprintf(outString, "%s%d", base_string, number);
      char buf[sizeof(int) * 6 + 2];

      for (int i = 0; i < n; i++)
      {
        //networkList.append(WiFi.SSID(i).c_str());
        //networkList.append("\n");

        //snprintf(buf, sizeof buf, "%d", WiFi.channel(i));
        //networkList.append(buf);
        //networkList.append("\n");

        //snprintf(buf, sizeof buf, "%d", WiFi.RSSI(i));
        //networkList.append(buf);
        //networkList.append("\n");

        //snprintf(buf, sizeof buf, "%d", WiFi.encryptionType(i));
        //networkList.append(buf);
        //networkList.append("\n");
      }

      //pNetworksCharacteristic->setValue(networkList.data());
      //pNetworksCharacteristic->notify();

      //WiFi.scanDelete();
      scanStarted = false;
    }

    delay(1000); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
  }
  else if (oldDeviceConnected)
  {
    delay(500);                  // give the bluetooth stack the chance to get things ready
    //pServer->startAdvertising(); // restart advertising
    LOGGER.println("BLE Advertising");
    oldDeviceConnected = false;
  }
  else
  {
    delay(1000);
  }

  // connecting
  if (deviceConnected && !oldDeviceConnected)
  {
    // do stuff here on connecting
    oldDeviceConnected = true;
  }
}

void setup()
{
  setupLogger();

  //ledcSetup(ledChannel, freq, resolution);
  //ledcAttachPin(FADE_PIN, ledChannel);

  //Settings mySettings;
  settings = new Settings(); //&mySettings;

  //pinMode(LED_PIN, OUTPUT);
  //pinMode(BTN_PIN, INPUT);

  settings->checkReset();

  //digitalWrite(LED_PIN, HIGH);

  stageLoop = &setupStageLoop;

  setupStageInit();
}

void loop()
{
  stageLoop();
}

/*
#ifdef __cplusplus
extern "C" {
#endif
void app_main(void)
{
    setup();
    
    for( ;; )
    {
        stageLoop();
    }

    return;
}
#ifdef __cplusplus
}
#endif
*/

/*
void app_main()
{
    // 2. A configuration structure of type esp_event_loop_args_t is needed to specify the properties of the loop to be
    // created. A handle of type esp_event_loop_handle_t is obtained, which is needed by the other APIs to reference the loop
    // to perform their operations on.
    esp_event_loop_args_t loop_args = {
        .queue_size = ...,
        .task_name = ...
        .task_priority = ...,
        .task_stack_size = ...,
        .task_core_id = ...
    };

    esp_event_loop_handle_t loop_handle;

    esp_event_loop_create(&loop_args, &loop_handle);

    // 3. Register event handler defined in (1). MY_EVENT_BASE and MY_EVENT_ID specifies a hypothetical
    // event that handler run_on_event should execute on when it gets posted to the loop.
    esp_event_handler_register_with(loop_handle, MY_EVENT_BASE, MY_EVENT_ID, run_on_event, ...);

    ...

    // 4. Post events to the loop. This queues the event on the event loop. At some point in time
    // the event loop executes the event handler registered to the posted event, in this case run_on_event.
    // For simplicity sake this example calls esp_event_post_to from app_main, but posting can be done from
    // any other tasks (which is the more interesting use case).
    esp_event_post_to(loop_handle, MY_EVENT_BASE, MY_EVENT_ID, ...);

    ...

    // 5. Unregistering an unneeded handler
    esp_event_handler_unregister_with(loop_handle, MY_EVENT_BASE, MY_EVENT_ID, run_on_event);

    ...

    // 6. Deleting an unneeded event loop
    esp_event_loop_delete(loop_handle);
}
*/