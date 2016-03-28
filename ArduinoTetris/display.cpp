/*
 * 2016 Moon On Our Nation
 * port to Ucglib for support more device
 * 
*/
#include "display.h"
#include <SPI.h>
#include "Ucglib.h"

#ifdef ARDUINO_AVR_ESPLORA
  Ucglib_ST7735_18x128x160_HWSPI ucg(/*cd=*/ 0, /*cs=*/ 7, /*reset=*/ 1);
#elif ARDUINO_AVR_PROMICRO
  Ucglib_SSD1331_18x96x64_UNIVISION_HWSPI ucg(/*cd=*/ 19, /*cs=*/ 10, /*reset=*/ 18);
#else
  /* define your own setting here */
#endif

/*
  Hardware SPI Pins:
    Arduino Uno   sclk=13, data=11
    Arduino Due   sclk=76, data=75
    Arduino Mega  sclk=52, data=51

  >>> Please uncomment (and update) one of the following constructors. <<<
*/
//Ucglib8BitPortD ucg(ucg_dev_ili9325_18x240x320_itdb02, ucg_ext_ili9325_18, /* wr= */ 18, /* cd= */ 19, /* cs= */ 17, /* reset= */ 16);
//Ucglib8Bit ucg(ucg_dev_ili9325_18x240x320_itdb02, ucg_ext_ili9325_18, 0, 1, 2, 3, 4, 5, 6, 7, /* wr= */ 18, /* cd= */ 19, /* cs= */ 17, /* reset= */ 16);

//Ucglib4WireSWSPI ucg(ucg_dev_ili9325_18x240x320_itdb02, ucg_ext_ili9325_18, /*sclk=*/ 13, /*data=*/ 11, /*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);  // not working
//Ucglib4WireSWSPI ucg(ucg_dev_ili9325_spi_18x240x320, ucg_ext_ili9325_spi_18, /*sclk=*/ 13, /*data=*/ 11, /*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8); // not working
//Ucglib3WireILI9325SWSPI ucg(ucg_dev_ili9325_spi_18x240x320, ucg_ext_ili9325_spi_18, /*sclk=*/ 13, /*data=*/ 11, /*cs=*/ 10, /*reset=*/ 8);  // not working
//Ucglib3WireILI9325SWSPI ucg(ucg_dev_ili9325_18x240x320_itdb02, ucg_ext_ili9325_18, /*sclk=*/ 13, /*data=*/ 11, /*cs=*/ 10, /*reset=*/ 8); // not working

//Ucglib_ST7735_18x128x160_SWSPI ucg(/*sclk=*/ 13, /*data=*/ 11, /*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);
//Ucglib_ST7735_18x128x160_HWSPI ucg(/*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);

//Ucglib_ILI9163_18x128x128_SWSPI ucg(/*sclk=*/ 7, /*data=*/ 6, /*cd=*/ 5, /*cs=*/ 3, /*reset=*/ 4);
//Ucglib_ILI9163_18x128x128_HWSPI ucg(/*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);  /* HW SPI Adapter */

//Ucglib_ILI9341_18x240x320_SWSPI ucg(/*sclk=*/ 7, /*data=*/ 6, /*cd=*/ 5, /*cs=*/ 3, /*reset=*/ 4);
//Ucglib_ILI9341_18x240x320_SWSPI ucg(/*sclk=*/ 13, /*data=*/ 11, /*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);
//Ucglib_ILI9341_18x240x320_HWSPI ucg(/*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);
//Ucglib_ILI9341_18x240x320_SWSPI ucg(/*sclk=*/ 4, /*data=*/ 3, /*cd=*/ 6, /*cs=*/ 7, /*reset=*/ 5); /* Elec Freaks Shield */

//Ucglib_SSD1351_18x128x128_SWSPI ucg(/*sclk=*/ 13, /*data=*/ 11, /*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);
//Ucglib_SSD1351_18x128x128_HWSPI ucg(/*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);
//Ucglib_SSD1351_18x128x128_FT_SWSPI ucg(/*sclk=*/ 13, /*data=*/ 11, /*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);
//Ucglib_SSD1351_18x128x128_FT_HWSPI ucg(/*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);

//Ucglib_PCF8833_16x132x132_SWSPI ucg(/*sclk=*/ 13, /*data=*/ 11, /*cs=*/ 9, /*reset=*/ 8); /* linksprite board */
//Ucglib_PCF8833_16x132x132_HWSPI ucg(/*cs=*/ 9, /*reset=*/ 8); /* linksprite board */

//Ucglib_LD50T6160_18x160x128_6Bit ucg( /*d0 =*/ d0, /*d1 =*/ d1, /*d2 =*/ d2, /*d3 =*/ d3, /*d4 =*/ d4, /*d5 =*/ d5, /*wr=*/ wr, /*cd=*/ cd, /*cs=*/ cs, /*reset=*/ reset);
//Ucglib_LD50T6160_18x160x128_6Bit ucg( /*d0 =*/ 16, /*d1 =*/ 17, /*d2 =*/ 18, /*d3 =*/ 19, /*d4 =*/ 20, /*d5 =*/ 21, /*wr=*/ 14, /*cd=*/ 15); /* Samsung 160x128 OLED with 6Bit minimal interface with Due */
//Ucglib_LD50T6160_18x160x128_6Bit ucg( /*d0 =*/ 5, /*d1 =*/ 4, /*d2 =*/ 3, /*d3 =*/ 2, /*d4 =*/ 1, /*d5 =*/ 0, /*wr=*/ 7, /*cd=*/ 6); /* Samsung 160x128 OLED with 6Bit minimal interface with Uno */

//Ucglib_SSD1331_18x96x64_UNIVISION_SWSPI ucg(/*sclk=*/ 13, /*data=*/ 11, /*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);
//Ucglib_SSD1331_18x96x64_UNIVISION_HWSPI ucg(/*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);

//Ucglib_SEPS225_16x128x128_UNIVISION_SWSPI ucg(/*sclk=*/ 13, /*data=*/ 11, /*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);
//Ucglib_SEPS225_16x128x128_UNIVISION_HWSPI ucg(/*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);

void DISP::init (void)
{
  ucg.begin(UCG_FONT_MODE_TRANSPARENT);

#ifdef ARDUINO_AVR_ESPLORA
  ucg.setRotate270();
#elif ARDUINO_AVR_PROMICRO
#else
  /* define your own setting here */
#endif

  ucg.setFontPosTop();
  ucg.clearScreen();
}

void DISP::setColor(uint8_t idx, uint16_t rgb565)
{
  uint8_t r = (rgb565 >> 8) & 0b11111000;
  uint8_t g = (rgb565 >> 3) & 0b11111100;
  uint8_t b = (rgb565 << 3) & 0b11111000;
  ucg.setColor(idx, r, g, b);
}

void DISP::fillScreen(uint16_t color)
{
  setColor(0, color);
  ucg.drawBox(0, 0, MAX_X + 1, MAX_Y + 1);
}

void  DISP::drawHorizontalLine( uint16_t poX, uint16_t poY, uint16_t len, uint16_t color)
{
  setColor(0, color);
  ucg.drawHLine(poX, poY, len);
}

void DISP::drawVerticalLine( uint16_t poX, uint16_t poY, uint16_t len, uint16_t color)
{
  setColor(0, color);
  ucg.drawVLine(poX, poY, len);
}

void DISP::drawLine( uint16_t poX0, uint16_t poY0, uint16_t poX1, uint16_t poY1, uint16_t color)
{
  setColor(0, color);
  ucg.drawLine(poX0, poY0, poX1, poY1);
}

void DISP::drawRectangle(uint16_t poX, uint16_t poY, uint16_t w, uint16_t h, uint16_t color)
{
  setColor(0, color);
  ucg.drawFrame(poX, poY, w, h);
}

void DISP::drawRectangle(uint16_t poX, uint16_t poY, uint16_t w, uint16_t h, uint16_t color, byte thickness)
{
  setColor(0, color);
  for (int i = 0; i < thickness - 1; i++)
  {
    ucg.drawFrame(poX + i, poY + i, w - (i * 2), h - (i * 2));
  }
}

void DISP::fillRectangle(uint16_t poX, uint16_t poY, uint16_t w, uint16_t h, uint16_t color)
{
  setColor(0, color);
  ucg.drawBox(poX, poY, w, h);
}

void DISP::fillRectangleUseBevel(uint16_t poX, uint16_t poY, uint16_t w, uint16_t h, uint16_t color)
{
  setColor(0, color);
  ucg.drawBox(poX, poY, w, h);

  setColor(0, color | 0b1100011000011000); // lighter
  ucg.drawHLine(poX, poY, w - 1);

  setColor(0, color & 0b0011100111100111); // darker
  ucg.drawHLine(poX + 1, poY + h - 1, w - 1);
  ucg.drawVLine(poX + w - 1, poY + 1, h - 1);
}

void DISP::drawGradientBox(uint16_t poX, uint16_t poY, uint16_t w, uint16_t h, uint16_t color0, uint16_t color1, uint16_t color2, uint16_t color3)
{
  setColor(0, color0);
  setColor(1, color1);
  setColor(2, color2);
  setColor(3, color3);
  ucg.drawGradientBox(poX, poY, w, h);
}

void DISP::drawCircle(int poX, int poY, int r, uint16_t color)
{
  setColor(0, color);
  ucg.drawCircle(poX, poY, r, UCG_DRAW_ALL);
}

void DISP::fillCircle(int poX, int poY, int r, uint16_t color)
{
  setColor(0, color);
  ucg.drawDisc(poX, poY, r, UCG_DRAW_ALL);
}

void DISP::drawTriangle( int poX0, int poY0, int poX1, int poY1, int poX2, int poY2, uint16_t color)
{
  setColor(0, color);
  ucg.drawTriangle(poX0, poY0, poX1, poY1, poX2, poY2);
}

void DISP::setFontSize(uint8_t font_size)
{
  switch (font_size) {
    case 4:
      ucg.setFont(FONT_4X);
      break;
    default:
      ucg.setFont(FONT_1X);
      break;
  }
}

uint8_t DISP::drawNumber(long long_num, uint16_t poX, uint16_t poY, uint8_t font_size, uint16_t fgcolor)
{
  setColor(0, fgcolor);
  ucg.setPrintPos(poX, poY);
  setFontSize(font_size);
  return ucg.print(long_num);
}

uint8_t DISP::drawFloat(float floatNumber, uint16_t poX, uint16_t poY, uint8_t font_size, uint16_t fgcolor)
{
  setColor(0, fgcolor);
  ucg.setPrintPos(poX, poY);
  setFontSize(font_size);
  return ucg.print(floatNumber);
}

uint8_t DISP::drawString(char *str, uint16_t poX, uint16_t poY, uint8_t font_size, uint16_t fgcolor)
{
  setColor(0, fgcolor);
  ucg.setPrintPos(poX, poY);
  setFontSize(font_size);
  return ucg.print(str);
}

void DISP::drawStringWithShadow(char *str, uint16_t poX, uint16_t poY, uint8_t font_size, uint16_t fgcolor, uint16_t shcolor)
{
  drawString(str, poX + 1, poY + 1, font_size, shcolor);
  drawString(str, poX, poY, font_size, fgcolor);
}

void DISP::drawCenteredString(char *str, uint16_t poY, uint8_t font_size, uint16_t fgcolor)
{
  setFontSize(font_size);
  uint16_t len = ucg.getStrWidth(str);
  uint16_t left = (len > MAX_X) ? 0 : ((MAX_X - len) / 2);

  drawStringWithShadow(str, left, poY, font_size, fgcolor, GRAY1);
}

DISP Disp = DISP();
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

