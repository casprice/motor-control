/**
 * File: Driver.cpp
 */
#include <signal.h>
#include <rc/time.h>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <curses.h>
#include <unistd.h>

#include "Driver.hpp"
#include "Encoder.hpp"
#include "PID.hpp"
#include "library/gpio.h"
#include "library/pwm.h"
#include "library/util.h"
using namespace std;

#define STEP 1

static int running = 0;

/**
 * Handle Ctrl-C interrupt
 */
static void __signal_handler(__attribute__ ((unused)) int dummy) {
  running = 0;
  return;
}

/**
 *
 */
void setup(void) {
  // Curses initialization
  initscr(); // start curses
  cbreak();  // react to keys instantly w/o Enter
  noecho();  // don't echo input to screen
  keypad(stdscr, TRUE);  // register arrow keys
  nodelay(stdscr, TRUE); // don't wait for keyboard input
}

/**
 * Main driver of the motor control for the snake.
 */
int main(int argc, char * argv[]) {
  char *endPtr;    // Used as second param of strtol
  int motor = 1;   // command line arg
  int setpoint = 0;   // angle we want motor to spin to
  char buf[20];    // buffer to print angle and position vals

  if (argc > 1) {
    // If too many arguments, print usage statement
    if (argc > 2) {
      cerr << "Error: Invalid number of arguments. Usage:" << endl;
      cerr << "  ./Driver [number]" << endl;
      cerr << "  number: the motor number" << endl;
      cerr << "    -- must be a valid integer" << endl;
      cerr << "    -- must be in the interval [1, 3]" << endl;
      return -1;
    }

    // Convert first argument to number and set as motor number
    motor = strtol(argv[1], &endPtr, 10);

    // If errno was set, motor is invalid number or contains non-numerical characters
    if (errno || *endPtr != '\0') {
      cerr << "Error: Invalid motor number. Aborting." << endl;
      return -1;
    }

    // Check that motor is in range
    if (motor < 1 || motor > 3) {
      cerr << "Error: Motor number out of range. Aborting." << endl;
      return -1;
    }
  }

  // Register keyboard interrupt
  signal(SIGINT, __signal_handler);
  running = 1;

  // Set up environment
  setup();

  // Encoder setup
  //shared_ptr<Encoder> enc1(new Encoder(2, 0x40));
  //shared_ptr<Encoder> enc2(new Encoder(2, enc_addr[0]));
  //shared_ptr<Encoder> enc3(new Encoder(3, 0x41));

  // PID setup
  shared_ptr<PID> pid1(new PID(motor, 0.08, 0.0, 0.0, NULL));
  pid1->setDuty(0.1);

  mvaddstr(0, 1, "Angle: 0");
  mvaddstr(1, 1, "Position: 0");

  // Main control loop
  while (running) {
    // Keyboard input
    int ch = getch();
    if (ch == KEY_LEFT) {
      setpoint -= STEP;
    }
    else if (ch == KEY_RIGHT) {
      setpoint += STEP;
    }
    else if (ch == 'q') {
      running = 0;
      continue;
    }

    clear(); // refresh the terminal window
/*
    // Exit the control loop if no longer reading encoder.
    if (enc2->calcRotation() == -1) {
      running = 0;
      continue;
    }
*/
    //double angle = enc2->getAngle();
    //pid1.updatePWM(setpoint, true);

    sprintf(buf, "Angle: %d", setpoint);
    mvaddstr(0, 1, buf);

    memset(buf, '\0', sizeof(char));
    sprintf(buf, "Motor #%d", motor);
    mvaddstr(1, 1, buf);

    /*
    memset(buf, '\0', sizeof(char));
    sprintf(buf, "Position: %f", angle);
    mvaddstr(1, 1, buf);
*/
    sleep(DT);
  }

  endwin();  // restore terminal from curses

  return 0;
}
