#ifndef LOGGER_H
#define LOGGER_H

#include <U8x8lib.h>
#include <U8g2lib.h>

// the OLED used
//U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 16, /* clock=*/ 15, /* data=*/ 4);
U8G2LOG u8g2log;

//U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);
//U8X8LOG u8x8log;

// Define the dimension of the U8x8log window
#define U8LOG_WIDTH 18
#define U8LOG_HEIGHT 8

// Allocate static memory for the U8x8log window
uint8_t u8log_buffer[U8LOG_WIDTH * U8LOG_HEIGHT];

#define LOGGER u8g2log
//#define LOGGER Serial

class Logger {
    public:
        Logger();

};

void setupLogger() {
  u8g2.setBusClock(400000);
  u8g2.begin();
  u8g2.setFont(u8g2_font_synchronizer_nbp_tr);
  u8g2log.begin(u8g2, U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buffer);
  u8g2log.setLineHeightOffset(2);
  u8g2log.setRedrawMode(0);
}

#endif
