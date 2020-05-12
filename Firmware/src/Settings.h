#ifndef SETTINGS_H
#define SETTINGS_H

#include <SettingsMainConfig.h>
#include <SettingsWifiConfig.h>
//#include <Preferences.h>

class Settings
{
public:
  Settings();

  //void start();
  void checkReset();
  void updateWifiConfig();

private:
  //Preferences prefs;
};

#include "Board.h"
#include "Logger.h"
#include "WifiManager.h"

bool doReset = false;

std::string networkSsid;
std::string networkAuth;

Settings::Settings()
{
  LOGGER.println("prefs begin app");
  //prefs.begin("app");

  LOGGER.println("getBytes cfg");
  //prefs.getBytes("cfg", &mainConfig, sizeof(mainConfig_t));

  LOGGER.print("cfg: ");
  //LOGGER.println(mainConfig.version);
  LOGGER.print("wifi: ");
  //LOGGER.println(mainConfig.wifiConfigured);

  //prefs.getBytes("w", &wifiConfig, sizeof(wifiConfig_t));
  LOGGER.println("ssid: ");
  LOGGER.println(wifiConfig.ssid);
}

void abortReset()
{
  //detachInterrupt(BTN_PIN);
  LOGGER.println("Abort reset.");
  doReset = false;
}

void Settings::checkReset()
{
  /*
  int buttonState = digitalRead(BTN_PIN);

  if (buttonState == HIGH)
  {
    doReset = true;
    LOGGER.println("Reset button active.");
    attachInterrupt(BTN_PIN, abortReset, LOW);
    delay(5000);

    detachInterrupt(BTN_PIN);
    LOGGER.print("doReset: ");
    LOGGER.println(doReset);

    if (doReset)
    {
      LOGGER.println("Clearing settings...");
      //prefs.begin("app");
      //prefs.clear();
      //prefs.end();
      LOGGER.println("Settings cleared.\nRestart in 2s...");

      delay(2000);
      //ESP.restart();
    }
  }
  */
}

void Settings::updateWifiConfig()
{
  LOGGER.println("Updating WiFi config...");

  //strcpy(wifiConfig.ssid, networkSsid.c_str());
  //strcpy(wifiConfig.auth, networkAuth.c_str());

  //prefs.putBytes("w", &wifiConfig, sizeof(wifiConfig_t));

  // TODO: Need to verify before saving!
  //mainConfig.wifiConfigured = networkSsid.size() != 0 && networkAuth.size() != 0;
  mainConfig.wifiConfigured = wifiVerified;

  //prefs.putBytes("cfg", &mainConfig, sizeof(mainConfig_t));

  LOGGER.println("WiFi config updated.");

  /*
  if(mainConfig.wifiConfigured) {
    verifyWifiAndConnect();
  }*/
}

Settings *settings;

#endif