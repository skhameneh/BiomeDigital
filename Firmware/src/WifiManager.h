#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <StageManager.h>
#include <Sun.h>
#include <TimeManager.h>
//#include <WiFi.h>
#include <WiFiMulti.h>
#include <Settings.h>

WiFiMulti wifiMulti;

bool scanStarted = false;

const char* ntpServer = "pool.ntp.org";

bool verifyWifiAndConnect() {
  //networkSsid = std::string(wifiConfig.ssid);
  //networkAuth = std::string(wifiConfig.auth);

  //WiFi.begin(wifiConfig.ssid, wifiConfig.auth);
  LOGGER.println(wifiConfig.ssid);
  LOGGER.println(strlen(wifiConfig.ssid));
  LOGGER.println(strlen(wifiConfig.auth));

  if(strlen(wifiConfig.auth) != 0 && strlen(wifiConfig.ssid) != 0) {
    WiFi.disconnect();

    LOGGER.printf("Connecting to %s ", wifiConfig.ssid);
    wifiMulti.addAP(wifiConfig.ssid, wifiConfig.auth);
  }

  /*
  while(wifiMulti.run() != WL_CONNECTED) {
      LOGGER.print(".");
      delay(100);
  }
  LOGGER.println("\n");
  */

  //btStop();

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

  delay(500);

  return true;
}

#endif