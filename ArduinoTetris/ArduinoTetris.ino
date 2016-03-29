/*
 * 2016 Moon On Our Nation
 * port to Ucglib for support more device
 * 
    Arduino Tetris
    Copyright (C) 2015  Jo�o Andr� Esteves Vila�a

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

#include <SPI.h>
#include "Ucglib.h"
#include "display.h"

#ifdef ARDUINO_AVR_ESPLORA
  #include <Esplora.h>
#endif

#include "joystick.cpp"
#include "beeping.cpp"
#include "tetris.cpp"
#include "sequencer.cpp"

Tetris t;

void setup() {
  t.init();

  Sequencer::init();

  Disp.init();  // init display library

  // play turnon sound
  Beeping::turnOn();

  // initialize joystick
  Joystick::init();
}

void loop() {
  // title screen
  t.drawPreGameScreen();

  // wait a bit before playing sounds
  delay(700);

  // start playing tetris theme
  Sequencer::start();

  // wait for click
  while (!Joystick::fire());

  // stop playing tetris theme
  Sequencer::stop();

  // load game
  t.run();

  // game ended
  t.gameOver();
}

ISR(TIMER1_COMPA_vect) {
  // sequencer plays tetris theme
  Sequencer::play();
}

