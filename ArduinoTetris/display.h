/*
 * 2016 Moon On Our Nation
 * port to Ucglib for support more device
 * 
*/
#ifndef DISP_h
#define DISP_h
#include <Arduino.h>

#include <SPI.h>
#include "Ucglib.h"

#ifdef ARDUINO_AVR_ESPLORA
  //display resolution 160*128
  #define MIN_X 0
  #define MIN_Y 0
  #define MAX_X 159
  #define MAX_Y 127
  #define FONT_4X ucg_font_osb29_hr
  #define FONT_4X_WIDTH 18
  #define FONT_4X_HEIGHT 40
  #define FONT_1X ucg_font_helvB08_tr
  #define FONT_1X_WIDTH 5
  #define FONT_1X_HEIGHT 11
#elif ARDUINO_AVR_LEONARDO
  //display resolution 96*64
  #define MIN_X 0
  #define MIN_Y 0
  #define MAX_X 95
  #define MAX_Y 63
  #define FONT_4X ucg_font_osb18_hr
  #define FONT_4X_WIDTH 10
  #define FONT_4X_HEIGHT 26
  #define FONT_1X ucg_font_blipfest_07_tr
  #define FONT_1X_WIDTH 3
  #define FONT_1X_HEIGHT 6
#else
  /* define your own setting here */
#endif

//Basic Colors
#define BLACK       0x0000
#define BLUE        0x001f
#define CYAN        0x07ff
#define DARKCYAN    0x03EF      /*   0, 128, 128 */
#define DARKGREEN   0x03E0
#define DARKGREY    0x7BEF      /* 128, 128, 128 */
#define GRAY1       0x8410
#define GRAY2       0x4208
#define GRAY3       0x2104
#define GREEN       0x07e0
#define LIGHTGREEN  0xAFE5      /* 173, 255,  47 */
#define LIGHTGREY   0xC618      /* 192, 192, 192 */
#define MAGENTA     0xF81F      /* 255,   0, 255 */
#define MAROON      0x7800      /* 128,   0,   0 */
#define NAVY        0x000F      /*   0,   0, 128 */
#define OLIVE       0x7BE0      /* 128, 128,   0 */
#define ORANGE      0xFD20      /* 255, 165,   0 */
#define PURPLE      0x780F      /* 128,   0, 128 */
#define RED         0xf800
#define WHITE       0xffff
#define YELLOW      0xffe0

extern uint8_t simpleFont[][8];

class DISP
{
  public:
    void init (void);
    void setColor(uint8_t idx, uint16_t color565);
    void fillScreen(uint16_t color);

    void drawHorizontalLine(uint16_t poX, uint16_t poY, uint16_t len, uint16_t color);
    void drawVerticalLine(uint16_t poX, uint16_t poY, uint16_t len, uint16_t color);
    void drawLine(uint16_t poX0, uint16_t poY0, uint16_t poX1, uint16_t poY1, uint16_t color);
    void drawRectangle(uint16_t poX, uint16_t poY, uint16_t w, uint16_t h, uint16_t color);
    void drawRectangle(uint16_t poX, uint16_t poY, uint16_t w, uint16_t h, uint16_t color, byte thickness);
    void fillRectangle(uint16_t poX, uint16_t poY, uint16_t w, uint16_t h, uint16_t color);
    void fillRectangleUseBevel(uint16_t poX, uint16_t poY, uint16_t w, uint16_t h, uint16_t color);
    void drawGradientBox(uint16_t poX, uint16_t poY, uint16_t w, uint16_t h, uint16_t color0, uint16_t color1, uint16_t color2, uint16_t color3);

    void drawCircle(int poX, int poY, int r, uint16_t color);
    void fillCircle(int poX, int poY, int r, uint16_t color);
    void drawTriangle(int poX0, int poY0, int poX1, int poY1, int poX2, int poY2, uint16_t color);

    void setFontSize(uint8_t font_size);
    uint8_t fontWidth(uint8_t font_size);
    uint8_t drawNumber(long long_num, uint16_t poX, uint16_t poY, uint8_t font_size, uint16_t fgcolor);
    uint8_t drawFloat(float floatNumber, uint16_t poX, uint16_t poY, uint8_t font_size, uint16_t fgcolor);
    uint8_t drawString(char *string, uint16_t poX, uint16_t poY, uint8_t font_size, uint16_t fgcolor);
    void drawStringWithShadow(char *string, uint16_t poX, uint16_t poY, uint8_t font_size, uint16_t fgcolor, uint16_t shcolor);
    void drawCenteredString(char *string, uint16_t poY, uint8_t font_size, uint16_t fgcolor);
};

extern DISP Disp;
#endif

