#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <StageManager.h>
#include <Sun.h>
#include <TimeManager.h>
//#include <cmd_wifi.h>
#include <esp_wifi.h>
//#include <WiFi.h>
//#include <WiFiMulti.h>
#include <Settings.h>

//WiFiMulti wifiMulti;
//WiFiClient client;

bool scanStarted = false;

const char *ntpServer = "pool.ntp.org";

/*
void WiFiEvent(WiFiEvent_t event)
{
  LOGGER.print("Wifi: ");
  LOGGER.println(event);
  
  switch (event)
  {
  case SYSTEM_EVENT_AP_START:
    //can set ap hostname here
    //WiFi.softAPsetHostname(wifiConfig.ssid);
    //enable ap ipv6 here
    //WiFi.softAPenableIpV6();
    break;

  case SYSTEM_EVENT_STA_START:
    //set sta hostname here
    //WiFi.setHostname(wifiConfig.ssid);
    break;
  case SYSTEM_EVENT_STA_CONNECTED:
    //enable sta ipv6 here
    //LOGGER.println("Connected");

    //LOGGER.println("WiFi connected");
    //LOGGER.print("IP: ");
    //OGGER.println(WiFi.localIP());

    //configTime(CST_OFFSET * 60 * 60, 3600, ntpServer);
    //WiFi.enableIpV6();
    //LOGGER.println("Enabled ipv6");
    break;
  case SYSTEM_EVENT_STA_DISCONNECTED:
    WiFi.begin(wifiConfig.ssid, wifiConfig.auth);
  case SYSTEM_EVENT_AP_STA_GOT_IP6:
    //both interfaces get the same event
    LOGGER.print("STA IPv6: ");
    LOGGER.println(WiFi.localIPv6());
    //Serial.print("AP IPv6: ");
    //Serial.println(WiFi.softAPIPv6());
    //wifiVerified = true;
    break;
  default:
    break;
  }
  
}
*/

//int status = WL_IDLE_STATUS;
//IPAddress server(74, 125, 115, 105); // Google

bool verifyWifiAndConnect()
{
  /*
  //WiFi.disconnect(true);
  esp_wifi_set_mode(WIFI_MODE_STA);
  //esp_wifi_set_bandwidth(ESP_IF_WIFI_STA, WIFI_BW_HT20);
  //WiFi.setSleepMode(WIFI_NONE_SLEEP)

  delay(500);
  //networkSsid = std::string(wifiConfig.ssid);
  //networkAuth = std::string(wifiConfig.auth);

  WiFi.onEvent(WiFiEvent);
  WiFi.enableIpV6();
  //status = WiFi.begin(wifiConfig.ssid, wifiConfig.auth);

  LOGGER.println(wifiConfig.ssid);

  WiFi.setHostname("BiomeDigital");
  delay(500);
  //WiFi.setPhyMode(WIFI_PHY_MODE_11N);
  //WiFi.mode(WIFI_STA);
  WiFi.begin(wifiConfig.ssid, wifiConfig.auth); while (WiFi.status() != WL_CONNECTED) { delay(100); Serial.print("."); }

  //btStop();
  LOGGER.println("Connected.");
  */

  //wifiMulti.addAP(wifiSsid, wifiAuth);

  /*
  while (wifiMulti.run() != WL_CONNECTED)
  {
    LOGGER.print(".");
    delay(500);
  }
  */
  LOGGER.println("\n");
  //LOGGER.println(wifiConfig.auth);
  //LOGGER.println(strlen(wifiConfig.ssid));
  //LOGGER.println(strlen(wifiConfig.auth));

  /*
  if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET /search?q=arduino HTTP/1.0");
    client.println();
  }
  */
  /*
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  */

  /*
  if (strlen(wifiConfig.auth) != 0 && strlen(wifiConfig.ssid) != 0)
  {
    //WiFi.disconnect();
    WiFi.disconnect(true);
    WiFi.onEvent(WiFiEvent);

    LOGGER.printf("Connecting to %s ", wifiConfig.ssid);
    WiFi.begin(wifiConfig.ssid, wifiConfig.auth);
    //wifiMulti.addAP(wifiConfig.ssid, wifiConfig.auth);
  }
  */

  //btStop();
  /*
  if(mainConfig.wifiConfigured) {
    while(wifiMulti.run() != WL_CONNECTED) {
      LOGGER.print(".");
      delay(100);
    }
    LOGGER.println("");

    LOGGER.println("WiFi connected");
    LOGGER.print("IP: ");
    LOGGER.println(WiFi.localIP());

    configTime(CST_OFFSET * 60 * 60, 3600, ntpServer);
  }
  */

  //delay(500);

  return true;
}

#endif