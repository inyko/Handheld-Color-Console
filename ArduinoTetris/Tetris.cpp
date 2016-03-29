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

#ifndef TETRISCPP
#define TETRISCPP

#include "display.h"
#include "Arduino.h"
#include "joystick.cpp"
#include "beeping.cpp"

#ifdef ARDUINO_AVR_ESPLORA
  #define RANDOM_PIN        27
  #define RANDOM_REFERENCE  4
#elif ARDUINO_AVR_PROMICRO
  #define RANDOM_PIN        27
  #define RANDOM_REFERENCE  4
#else
  #define RANDOM_PIN        16
  #define RANDOM_REFERENCE  2
#endif

#define MIN(X, Y)             (((X) < (Y)) ? (X) : (Y))

#define BOARD_WIDTH           11
#define BOARD_HEIGHT          20

#define BLOCK_SIZE            MIN(((MAX_X - 2) / BOARD_WIDTH), ((MAX_Y - 2) / BOARD_HEIGHT))

#define BOARD_TOP             (((MAX_Y + 1) - BOARD_HEIGHT * BLOCK_SIZE) / 2)
#define BOARD_BOTTOM          ((BOARD_TOP + BOARD_HEIGHT * BLOCK_SIZE) - 1)

#define MARGIN                MIN(BOARD_TOP, 2)

#define NEXT_BOX_WIDTH (BLOCK_SIZE * 6)
#define NEXT_BOX_HEIGHT (BLOCK_SIZE * 5)
#define NEXT_BOX_LEFT (MAX_X - MARGIN - NEXT_BOX_WIDTH)
#define NEXT_BOX_TOP MARGIN

#define GAME_OVER_TOP ((MAX_Y - (FONT_4X_HEIGHT * 2) - MARGIN) / 2)

#define BOARD_RIGHT           (NEXT_BOX_LEFT - (MARGIN * 2) - 2)
#define BOARD_LEFT            (BOARD_RIGHT - (BOARD_WIDTH * BLOCK_SIZE) + 1 )

#define SCORE_BOARD_WIDTH (BOARD_LEFT - (MARGIN * 3))
#define SCORE_BOARD_HEIGHT ((MARGIN *5) + (FONT_1X_HEIGHT * 4))

#define PIT_COLOR             WHITE
#define BG_COLOR              BLACK

#define DROP_WAIT_INIT        1100

#define INPUT_WAIT_ROT        300
#define INPUT_WAIT_MOVE       500

#define INPUT_WAIT_NEW_SHAPE  400

// used to clear the position from the screen
typedef struct Backup {
  byte x, y, rot;
};


class Tetris
{
    // shapes definitions

    byte l_shape[4][4][2] {
      {{0, 0}, {0, 1}, {0, 2}, {1, 2}},
      {{0, 1}, {1, 1}, {2, 0}, {2, 1}},
      {{0, 0}, {1, 0}, {1, 1}, {1, 2}},
      {{0, 0}, {0, 1}, {1, 0}, {2, 0}},
    };

    byte j_shape[4][4][2] {
      {{1, 0}, {1, 1}, {0, 2}, {1, 2}},
      {{0, 0}, {1, 0}, {2, 0}, {2, 1}},
      {{0, 0}, {1, 0}, {0, 1}, {0, 2}},
      {{0, 0}, {0, 1}, {1, 1}, {2, 1}},
    };

    byte o_shape[1][4][2] {
      { {0, 0}, {0, 1}, {1, 0}, {1, 1}}
    };

    byte s_shape[2][4][2] {
      {{0, 1}, {1, 0}, {1, 1}, {2, 0}},
      {{0, 0}, {0, 1}, {1, 1}, {1, 2}}
    };

    byte z_shape[2][4][2] {
      {{0, 0}, {1, 0}, {1, 1}, {2, 1}},
      {{1, 0}, {0, 1}, {1, 1}, {0, 2}}
    };

    byte t_shape[4][4][2] {
      {{0, 0}, {1, 0}, {2, 0}, {1, 1}},
      {{0, 0}, {0, 1}, {1, 1}, {0, 2}},
      {{1, 0}, {0, 1}, {1, 1}, {2, 1}},
      {{1, 0}, {0, 1}, {1, 1}, {1, 2}}
    };

    byte i_shape[2][4][2] {
      {{0, 0}, {1, 0}, {2, 0}, {3, 0}},
      {{0, 0}, {0, 1}, {0, 2}, {0, 3}}
    };

    // All game shapes and their colors

    byte *all_shapes[7] = {l_shape[0][0], j_shape[0][0], o_shape[0][0], s_shape[0][0], z_shape[0][0], t_shape[0][0], i_shape[0][0]};

    unsigned int colors[7] = {ORANGE, BLUE, YELLOW, GREEN, RED, MAGENTA, CYAN};


    // how many rotated variations each shape has

    byte shapes[7] = {4, 4, 1, 2, 2, 4, 2};

    // game progress

    int lines, level;

    // current shapes

    byte current;

    // tetris guidelines have all 7 shapes
    // selected in sequence to avoid
    // long runs without a shape

    byte next[7];
    byte next_c;

    unsigned long lastInput, lastDrop;

    byte board[BOARD_HEIGHT][BOARD_WIDTH];

    byte x, y, rot;
    Backup old;

    boolean newShape;

    int dropWait;

  public:
    Tetris() : newShape(true), lines(0)
    {
    }

    void init()
    {
      // RANDOM_REFERENCE analog pin MUST NOT be connected to anything...
      pinMode(RANDOM_PIN, INPUT);
      randomSeed(analogRead(RANDOM_REFERENCE));
      random(7);
      random(7);
    }

    void drawPreGameScreen()
    {
      Disp.drawGradientBox(0, 0, MAX_X + 1, MAX_Y + 1, RED, MAROON, GREEN, DARKGREEN);
      Disp.drawCenteredString("Tetris", MARGIN * 4, 4, BLUE);
      Disp.drawCenteredString("Click to play", MAX_Y / 3 * 2, 1, BLACK);
      Disp.drawCenteredString("http://vilaca.eu", MAX_Y - MARGIN - FONT_1X_HEIGHT, 1, PURPLE);
    }

    void run()
    {
      // clear board
      for ( int i = 0; i < BOARD_WIDTH; i++ )
      {
        for ( int j = 0; j < BOARD_HEIGHT; j++)
        {
          board[j][i] = 0;
        }
      }

      //next shape

      randomizer();

      // initialize game logic

      lastInput = 0;
      lastDrop = 0;
      dropWait = DROP_WAIT_INIT;
      level = 1;
      newShape = true;

      // draw background
      Disp.drawGradientBox(0, 0, MAX_X + 1, MAX_Y + 1, BLUE, CYAN, RED, GREEN);

      // draw board
      Disp.drawRectangle(BOARD_LEFT - 1, BOARD_TOP - 1, BOARD_RIGHT - BOARD_LEFT + 3, BOARD_BOTTOM - BOARD_TOP + 3, PIT_COLOR);
      Disp.fillRectangle(BOARD_LEFT, BOARD_TOP, BOARD_RIGHT - BOARD_LEFT + 1, BOARD_BOTTOM - BOARD_TOP + 1, BG_COLOR);

      scoreBoard();

      do {

        // get clock
        const unsigned long now = millis();

        // display new shape
        if ( newShape )
        {
          Joystick::waitForRelease(INPUT_WAIT_NEW_SHAPE);
          newShape = false;

          // a new shape enters the game
          chooseNewShape();

          // draw next box
          Disp.drawRectangle(NEXT_BOX_LEFT - 1, NEXT_BOX_TOP - 1, NEXT_BOX_WIDTH + 2, NEXT_BOX_HEIGHT + 2, PIT_COLOR);
          Disp.fillRectangle(NEXT_BOX_LEFT, NEXT_BOX_TOP, NEXT_BOX_WIDTH, NEXT_BOX_HEIGHT, BLACK);
          byte *shape = all_shapes[next[next_c]];
          for ( int i = 0; i < 4; i++ )
          {
            byte *block = shape + i * 2;
            Disp.fillRectangleUseBevel(
              NEXT_BOX_LEFT + BLOCK_SIZE + block[0] * BLOCK_SIZE,
              NEXT_BOX_TOP + BLOCK_SIZE + block[1] * BLOCK_SIZE,
              BLOCK_SIZE,
              BLOCK_SIZE,
              colors[next[next_c]]);
          }

          // check if new shape is placed over other shape(s)
          // on the board
          if ( touches(0, 0, 0 ))
          {
            // draw shape to screen
            draw();
            return;
          }

          // draw shape to screen
          draw();
        }
        else
        {
          // check if enough time has passed since last time the shape
          // was moved down the board
          if ( now - lastDrop > dropWait || Joystick::getY() > Joystick::NEUTRAL)
          {
            // update clock
            lastDrop = now;
            moveDown();
          }
        }

        if (!newShape)
        {
          userInput(now);
        }

      } while (true);
    }

    void gameOver()
    {
      Disp.drawCenteredString("GAME", GAME_OVER_TOP, 4, RED);
      Disp.drawCenteredString("OVER", GAME_OVER_TOP + FONT_4X_HEIGHT + MARGIN, 4, RED);

      Beeping::beep(600, 200);
      delay(300);
      Beeping::beep(600, 200);
      delay(300);
      Beeping::beep(200, 600);
      delay(1500);
      Joystick::waitForClick();
    }

  private:

    void chooseNewShape()
    {
      current = next[next_c];

      next_c++;

      if ( next_c == 7 )
      {
        randomizer();
      }

      // new shape must be positioned at the middle of
      // the top of the board
      // with zero rotation

      rot = 0;
      y = 0;
      x = BOARD_WIDTH / 2;

      old.rot = rot;
      old.y = y;
      old.x = x;
    }

    void userInput(unsigned long now)
    {
      unsigned long waited = now - lastInput;

      int jx = Joystick::getX();

      int move = INPUT_WAIT_MOVE / jx;

      if ( jx < Joystick::NEUTRAL && waited > -move)
      {
        if  (x > 0 && !touches(-1, 0, 0))
        {
          x--;
          lastInput = now;
          draw();
        }
      }
      else if ( jx > Joystick::NEUTRAL && waited > move )
      {
        if ( x < BOARD_WIDTH && !touches(1, 0, 0))
        {
          x++;
          lastInput = now;
          draw();
        }
      }

      if ( Joystick::fire()  && waited > INPUT_WAIT_ROT )
      {
        rotate(now);
      }

      int my = Joystick::getY();
      if (( my == -Joystick::HARD && waited > INPUT_WAIT_ROT ) || ( my == -Joystick::HARDER && waited > INPUT_WAIT_ROT / 2 ))
      {
        if (Joystick::getY() < 0)
        {
          rotate(now);
        }
      }
    }

    void rotate(unsigned long now)
    {
      if (!touches(0, 0, 1))
      {
        rot++;
        rot %= shapes[current];
        lastInput = now;
        draw();
      }
    }

    void moveDown()
    {
      // prepare to move down
      // check if board is clear bellow

      if ( touches(0, 1, 0 ))
      {
        // moving down touches another shape
        newShape = true;

        // this shape wont move again
        // add it to the board

        byte *shape = all_shapes[current];
        for ( int i = 0; i < 4; i++ )
        {
          byte *block = (shape + (rot * 4 + i) * 2);
          board[block[1] + y][block[0] + x] = current + 1;
        }

        // check if lines were made

        score();
        Beeping::beep(1500, 25);
      }
      else
      {
        // move shape down
        y += 1;
        draw();
      }
    }

    void draw()
    {
      byte *shape = all_shapes[current];
      for ( int i = 0; i < 4; i++ )
      {
        byte *block = (shape + (rot * 4 + i) * 2);

        Disp.fillRectangleUseBevel(
          BOARD_LEFT + block[0] * BLOCK_SIZE + BLOCK_SIZE * x,
          BOARD_TOP + block[1] * BLOCK_SIZE + BLOCK_SIZE * y,
          BLOCK_SIZE,
          BLOCK_SIZE,
          colors[current]);

        board[block[1] + y][block[0] + x] = 255;
      }

      // erase old
      for ( int i = 0; i < 4; i++ )
      {
        byte *block = (shape + (old.rot * 4 + i) * 2);

        if ( board[block[1] + old.y][block[0] + old.x] == 255 )
          continue;

        Disp.fillRectangle(
          BOARD_LEFT + block[0] * BLOCK_SIZE + BLOCK_SIZE * old.x,
          BOARD_TOP + block[1] * BLOCK_SIZE + BLOCK_SIZE * old.y,
          BLOCK_SIZE,
          BLOCK_SIZE,
          BG_COLOR);
      }

      for ( int i = 0; i < 4; i++ )
      {
        byte *block = (shape + (rot * 4 + i) * 2);
        board[block[1] + y][block[0] + x] = 0;
      }

      old.x = x;
      old.y = y;
      old.rot = rot;
    }

    boolean touches(int xi, int yi, int roti)
    {
      byte *shape = all_shapes[current];
      for ( int i = 0; i < 4; i++ )
      {
        byte *block = (shape + (((rot + roti) % shapes[current]) * 4 + i) * 2);

        int x2 = x + block[0] + xi;
        int y2 = y + block[1] + yi;

        if ( y2 == BOARD_HEIGHT || x2 == BOARD_WIDTH || board[y2][x2] != 0 )
        {
          return true;
        }
      }
      return false;
    }

    void score()
    {
      // we scan a max of 4 lines
      int ll;
      if ( y + 3 >= BOARD_HEIGHT )
      {
        ll = BOARD_HEIGHT - 1;
      }

      // scan board from current position
      for (int l = y; l <= ll; l++)
      {
        // check if there's a complete line on the board
        boolean line = true;

        for ( int c = 0; c < BOARD_WIDTH; c++ )
        {
          if (board[l][c] == 0)
          {
            line = false;
            break;
          }
        }

        if ( !line )
        {
          // move to next line
          continue;
        }

        Beeping::beep(3000, 50);

        lines++;

        if ( lines % 10 == 0 )
        {
          level++;
          dropWait /= 2;
        }

        scoreBoard();

        // move board down
        for ( int row = l; row > 0; row -- )
        {
          for ( int c = 0; c < BOARD_WIDTH; c++ )
          {
            byte v = board[row - 1][c];

            board[row][c] = v;

            if (v == 0)
            {
              Disp.fillRectangle(
                BOARD_LEFT + BLOCK_SIZE * c,
                BOARD_TOP + BLOCK_SIZE * row,
                BLOCK_SIZE,
                BLOCK_SIZE,
                BLACK);
            } else {
              Disp.fillRectangleUseBevel(
                BOARD_LEFT + BLOCK_SIZE * c,
                BOARD_TOP + BLOCK_SIZE * row,
                BLOCK_SIZE,
                BLOCK_SIZE,
                colors[v - 1]);
            }
          }
        }

        // clear top line
        for ( int c = 0; c < BOARD_WIDTH; c++ )
        {
          board[0][c] = 0;
        }
      }

      delay(350);
    }

    void scoreBoard()
    {
      Disp.drawRectangle(MARGIN - 1, MARGIN - 1, SCORE_BOARD_WIDTH + 2, SCORE_BOARD_HEIGHT + 2, PIT_COLOR);
      Disp.fillRectangle(MARGIN, MARGIN, SCORE_BOARD_WIDTH, SCORE_BOARD_HEIGHT, BLACK);
      Disp.drawString("Level", MARGIN * 2, MARGIN * 2, 1, YELLOW);
      Disp.drawNumber(level, MARGIN * 2, (MARGIN * 3) + FONT_1X_HEIGHT, 1, WHITE);
      Disp.drawString("Lines", MARGIN * 2, (MARGIN * 4) + (FONT_1X_HEIGHT * 2), 1, CYAN);
      Disp.drawNumber(lines, MARGIN * 2, (MARGIN * 5) + (FONT_1X_HEIGHT *3), 1, WHITE);
    }

    // create a sequence of 7 random shapes

    void randomizer()
    {
      // randomize 7 shapes

      for ( byte i = 0; i < 7; i ++)
      {
        boolean retry;
        byte shape;
        do
        {
          shape = random(7);

          // check if already in sequence

          retry = false;
          for ( int j = 0; j < i; j++)
          {
            if ( shape == next[j] )
            {
              retry = true;
              break;
            }
          }
        } while (retry);
        next[i] = shape;
      }
      next_c = 0;
    }
};

#endif

