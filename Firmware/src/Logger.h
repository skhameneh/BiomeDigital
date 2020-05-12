#ifndef LOGGER_H
#define LOGGER_H

#include "esp_log.h"

//#include <u8g.h>
//#include <U8g2lib.h>
//#include <U8glib.h>
//#include <U8glibPrint.h>

//u8g_dev_ssd1306_128x64_i2c u8g(U8G_I2C_OPT_NO_ACK)
//u8g_t u8g;
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);

/*
#include <U8x8lib.h>
#include <U8g2lib.h>

// the OLED used
//U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, 15, 4, 16);
//U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, clock, data, reset);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, 16, 15, 4);
U8G2LOG u8g2log;

//U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(15, 4, 16);
//U8X8LOG u8x8log;

// Define the dimension of the U8x8log window
#define U8LOG_WIDTH 18
#define U8LOG_HEIGHT 8

// Allocate static memory for the U8x8log window
uint8_t u8log_buffer[U8LOG_WIDTH * U8LOG_HEIGHT];

#define LOGGER u8g2log
*/

//#define LOGGER Serial

class Logger
{
public:
  Logger();
  void print(char const *);
  void println(char const *);
};

Logger::Logger()
{
}

static const char *TAG = "LOG";

void Logger::print(char const *message)
{
  ESP_LOGI(TAG, "%s", message);
}

void Logger::println(char const *message)
{
  ESP_LOGI(TAG, "%s", message);
}

Logger LOGGER;

//Logger LOGGER;

void setupLogger()
{
  //Serial.begin(115200);
  //initI2C(&u8g, &u8g_dev_ssd1306_128x64_i2c, U8G_I2C_OPT_NONE);

  //u8g.begin();
  //u8g.setFont(u8g_font_6x10);
  //u8g.drawStr(0, 20, "Hello world");
  //u8g_SetFont(&u8g, u8g_font_6x10);
  //u8g_DrawStr(&u8g, 0, 15, "Hello World!");
  /*
  u8g2.setBusClock(400000);
  u8g2.begin();
  u8g2.setFont(u8g2_font_synchronizer_nbp_tr);
  u8g2log.begin(u8g2, U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buffer);
  u8g2log.setLineHeightOffset(2);
  u8g2log.setRedrawMode(0);
  */
}

#endif
