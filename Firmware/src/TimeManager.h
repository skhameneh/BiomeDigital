#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <time.h>

#define CST_OFFSET      -6
#define DST_OFFSET      -5

void printLocalTime() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    LOGGER.println("Failed to obtain time");
    return;
  }
  LOGGER.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

#endif