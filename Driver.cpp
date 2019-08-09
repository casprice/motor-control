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
#define STEP 10
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

  // Set up curses environment
  setup();

  // GPIO/PWM setup
  PWM pwm("P9_14", 20000);
  GPIO dir(DIR_PIN);
  GPIO enable(ENABLE_PIN);
  GPIO pwm_pin(PWM_PIN);

  // Create new PID and Encoder controllers
  PID pidCtrl(0.8, 0.0003, 0.008);
  Encoder i2cDevice;
  
  mvaddstr(0, 1, "Angle: 0");
  mvaddstr(1, 1, "Position: -180");

  // main loop
  while (running) {
    int ch = getch();
    switch (ch) {
      case KEY_LEFT:
        value -= STEP;
        break;
      case KEY_RIGHT:
        value += STEP;
        break;
      case 'q':
        running = 0;
      default:
        continue;
    }

    value = clip(value, MIN_ANGLE, MAX_ANGLE);
    refresh(); // clear the terminal window
    clear();
    
    sprintf(buf, "Angle: %d", value);
    mvaddstr(0, 1, buf);

    i2cDevice.calcRotation();
    double angle = i2cDevice.getAngle();
    
    memset(buf, '\0', sizeof(char));
    sprintf(buf, "Position: %f", angle);
    mvaddstr(1, 1, buf);

    pidCtrl.updatePWM(pwm, angle, value);
    pidCtrl.updatePin(dir, false);

    counter++;
    if ((counter % 1000) == 0) pidCtrl.clearKi();
  }

  enable.setValue(GPIO::LOW);
  dir.setValue(GPIO::LOW);
  pwm_pin.setValue(GPIO::LOW);

  endwin();  // restore terminal from curses

  return 0;
}