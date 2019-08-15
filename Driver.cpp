/**
 * File: Driver.cpp
 */
#include <signal.h>
#include <rc/time.h>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <curses.h>

#include "Driver.hpp"
#include "Encoder.hpp"
#include "PID.hpp"
#include "library/gpio.h"
#include "library/pwm.h"
#include "library/util.h"
using namespace std;

#define FREQ 20000 // 20 kHz
#define STEP 1
#define MIN_ANGLE -175
#define MAX_ANGLE 175

static int running = 0;

/**
 * Handle Ctrl-C interrupt
 */
static void __signal_handler(__attribute__ ((unused)) int dummy) {
  running = 0;
  return;
}

/**
 * Ensure number doesn't exceed min or max angles
 */
double clip(double number, int min, int max) {
  if (number < min) number = min;
  if (number > max) number = max;
  return number;
}

/**
 * 
 */
void setup(void) {
  // Curses initialization
  initscr(); // start curses
  cbreak();  // react to keys instantly w/o Enter
  noecho();  // don't echo input to screen
  keypad(stdscr, TRUE); // register arrow keys
  nodelay(stdscr, TRUE);
}

/**
 * Main driver of the motor control for the snake.
 */
int main() {
  int value = 0;   // angle we want motor to spin to
  int counter = 0; // counter for Ki
  char buf[20];    // buffer to print angle and position vals

  // Register keyboard interrupt
  signal(SIGINT, __signal_handler);
  running = 1;

  // Set up environment
  setup();

  // GPIO/PWM setup
  PWM pwm(FREQ);
  GPIO dir1(M1_DIR);
  GPIO enable1(M1_ENABLE);
  GPIO dir2(M2_DIR);
  GPIO enable2(M2_ENABLE);
  GPIO dir3(M3_DIR);
  GPIO enable3(M3_ENABLE);

  // Encoder setup
  shared_ptr<Encoder> enc1(new Encoder(2, 0x40));
  //shared_ptr<Encoder> enc2(new Encoder(2, 0x41));

  // Create new PID and Encoder controllers
  PID pidCtrl(0.08, 0.0, 0.0, enc1);
  
  mvaddstr(0, 1, "Angle: 0");
  mvaddstr(1, 1, "Position: 0");

  // main loop
  while (running) {
    int ch = getch();
    if (ch == KEY_LEFT) {
      value -= STEP;
    }
    else if (ch == KEY_RIGHT) {
      value += STEP;
    }
    else if (ch == 'q') {
      running = 0;
    }

    value = clip(value, MIN_ANGLE, MAX_ANGLE);
    refresh(); // clear the terminal window
    clear();
    
    sprintf(buf, "Angle: %d", value);
    mvaddstr(0, 1, buf);

    enc1->calcRotation();
    //enc2->calcRotation();
    double angle = enc1->getAngle();
    
    memset(buf, '\0', sizeof(char));
    sprintf(buf, "Position: %f", angle);
    mvaddstr(1, 1, buf);

    memset(buf, '\0', sizeof(char));
    sprintf(buf, "updatePWM: %f", pidCtrl.updatePWM(&pwm, &dir3, value, true) );
    mvaddstr(2,1,buf);

    sleep(DT);
  }

  endwin();  // restore terminal from curses

  return 0;
}
