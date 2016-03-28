/*
 * 2016 Moon On Our Nation
 * port to Ucglib for support more device
 * 
    Arduino Tetris
    Copyright (C) 2015 João André Esteves Vilaça

    https://github.com/vilaca/Handheld-Color-Console

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef JOYSTICKCPP
#define JOYSTICKCPP

#include <Arduino.h>

#ifdef ARDUINO_AVR_ESPLORA
  #include <Esplora.h>
#elif ARDUINO_AVR_PROMICRO
  #define UP_PIN 2
  #define DW_PIN 3
  #define LF_PIN 4
  #define RG_PIN 5
  #define D_PIN 6
  #define A_PIN 7
  #define C_PIN 8
  #define B_PIN 9
#else
  // analog pins
  #define XPIN        0
  #define YPIN        1

  // digital pin
  #define FIREPIN     2

  // joystick center for both axis
  #define CENTER      512
#endif

class Joystick
{
  public:

    // joystick position constants

    static const int NEUTRAL = 0;
    static const int SOFT = 1;
    static const int HARD = 2;
    static const int HARDER = 3;

    static void init ()
    {
#ifdef ARDUINO_AVR_ESPLORA
#elif ARDUINO_AVR_PROMICRO
      pinMode(UP_PIN, INPUT_PULLUP);
      pinMode(DW_PIN, INPUT_PULLUP);
      pinMode(LF_PIN, INPUT_PULLUP);
      pinMode(RG_PIN, INPUT_PULLUP);
      pinMode(A_PIN, INPUT_PULLUP);
      pinMode(B_PIN, INPUT_PULLUP);
      pinMode(C_PIN, INPUT_PULLUP);
      pinMode(D_PIN, INPUT_PULLUP);
#else
      pinMode(FIREPIN, INPUT_PULLUP);
#endif
    }

    static int getX()
    {
#ifdef ARDUINO_AVR_ESPLORA
      return (Esplora.readJoystickX() / 128) * -1;
#elif ARDUINO_AVR_PROMICRO
      return (digitalRead(LF_PIN) == LOW) ? -2 : ((digitalRead(RG_PIN) == LOW) ? 2 : 0);
#else
      return getPosition(XPIN) * -1;
#endif
    }

    static int getY()
    {
#ifdef ARDUINO_AVR_ESPLORA
      return (Esplora.readJoystickY() / 128);
#elif ARDUINO_AVR_PROMICRO
      return (digitalRead(UP_PIN) == LOW) ? -2 : ((digitalRead(DW_PIN) == LOW) ? 2 : 0);
#else
      return getPosition(YPIN) * -1;
#endif
    }

    static boolean fire()
    {
#ifdef ARDUINO_AVR_ESPLORA
      return (Esplora.readButton(SWITCH_1) == LOW);
#elif ARDUINO_AVR_PROMICRO
      return (digitalRead(A_PIN) == LOW)
        | (digitalRead(B_PIN) == LOW)
        | (digitalRead(C_PIN) == LOW)
        | (digitalRead(D_PIN) == LOW);
#else
      return digitalRead(FIREPIN) == LOW;
#endif
    }

    static void waitForRelease()
    {
      while (fire());
    }

    static void waitForRelease(int howLong)
    {
      int c = 0;
      do
      {
        delay (10);
        c += 10;
      }
      while ((fire() || getY() != 0 || getX() != 0) && c < howLong);
    }

    static void waitForClick()
    {
      while (!fire());
    }

  private:

#ifdef ARDUINO_AVR_ESPLORA
#elif ARDUINO_AVR_PROMICRO
#else
    static int getPosition (int pin)
    {
      const int n = analogRead(pin) - CENTER;

      return n / 128;
    }
#endif
};

#endif


