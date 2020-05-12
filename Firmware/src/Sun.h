#ifndef SUN_H
#define SUN_H

#include <time.h>

//#include <SunSet.h>
#include <TimeManager.h>

//SunSet sun;

#define LATITUDE 32.7767
#define LONGITUDE 96.7970
#define SUN_BUFFER_MINUTES 30


double sunrise = -1;
double sunset = -1;
double bufferedSunrise = -1;
double bufferedSunset = -1;
double minutesInDay = -1;
double brightness = 0;

#define MINUTES_IN_DAY = 1440;

void updateBrightness() {
  /*
  struct tm timeinfo;

  while(!getLocalTime(&timeinfo)){
    LOGGER.println("Failed to obtain time");
    delay(1000);
    LOGGER.println("Retrying...");
  }

  double minutesPastMidnight = (timeinfo.tm_hour * 60) + timeinfo.tm_min + ((double)timeinfo.tm_sec / 60);
  bool isDaylight = minutesPastMidnight >= bufferedSunrise && minutesPastMidnight <= bufferedSunset;

  if(!isDaylight) {
    brightness = 0;
    return;
  }

  double dayLength = bufferedSunset - bufferedSunrise;
  double halfDayLength = dayLength / 2;
  double midDay = halfDayLength + bufferedSunrise;

  brightness = 1 - (fabs(minutesPastMidnight - midDay) / halfDayLength);
  
  LOGGER.println(brightness);
  //LOGGER.print(" @ ");
  //LOGGER.println((int)(brightness * dutyMax));
  */
}

void updateSunCycles() {
  /*
  struct tm timeinfo;

  while(!getLocalTime(&timeinfo)){
    LOGGER.println("Failed to obtain time");
    delay(1000);
    LOGGER.println("Retrying...");
  }

  LOGGER.println(&timeinfo, "%Y/%m/%d %H:%M:%S");
  //timeinfo.tm_hour;
  //timeinfo.tm_min;
  //timeinfo.tm_sec;

  sun.setPosition(LATITUDE, LONGITUDE, DST_OFFSET);
  sun.setCurrentDate(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday);
  //sun.setTZOffset(DST_OFFSET);

  sunrise = sun.calcSunrise();
  sunset = sun.calcSunset();

  bufferedSunrise = sunrise - SUN_BUFFER_MINUTES;
  bufferedSunset = sunset + SUN_BUFFER_MINUTES;

  LOGGER.print("Sunrise: ");
  LOGGER.print(sunrise);
  LOGGER.print(", Sunset: ");
  LOGGER.println(sunset);
  */
}

#endif
