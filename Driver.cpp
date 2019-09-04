/**
 * File: Driver.cpp
 */
#include <signal.h>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <curses.h>
#include <unistd.h>
#include <cmath>
#include <iostream>
#include <fstream>

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
  int motor = 3;   // command line arg
  double Kp = 0.5;
  double Kd = 0.0;
  int setpoint = 0;   // angle we want motor to spin to
  char buf[20];    // buffer to print angle and position vals
  ofstream angleReadings;
  angleReadings.open("angleReadings.csv");

  if (argc == 1) {
      cerr << "Error: Invalid number of arguments. Usage:" << endl;
      cerr << "  ./Driver [number] [Kp] [Ki]" << endl;
      cerr << "  number: the motor number" << endl;
      cerr << "    -- must be a valid integer" << endl;
      cerr << "    -- must be in the interval [1, 3]" << endl;
      cerr << "  Kp: the P in PID" << endl;
      cerr << "    -- must be a valid floating point number" << endl;
      cerr << "  Kd: the D in PID" << endl;
      cerr << "    -- must be a valid floating point number" << endl;
      return -1;
  } else {
    // If too many arguments, print usage statement
    if (argc > 4) {
      cerr << "Error: Invalid number of arguments." << endl;
      return -1;
    }

    // Convert first argument to number and set as motor number
    motor = strtol(argv[1], &endPtr, 10);
    if (argc > 2) {
        Kp = strtod(argv[2], &endPtr);
    }
    if (argc > 3) {
        Kd = strtod(argv[3], &endPtr);
    }
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

  auto start = clock();
  auto prevTime = clock();
  double duration;

  // Set up environment
  //setup();

  // Encoder setup
  I2CBus* theBus = new I2CBus(2);
  shared_ptr<Encoder> enc1(new Encoder(theBus, 0x40));
  //shared_ptr<Encoder> enc2(new Encoder(2, enc_addr[0]));
  //shared_ptr<Encoder> enc3(new Encoder(3, 0x41));

  // PID setup
  shared_ptr<PID> pid1(new PID(motor, Kp, 0.0, Kd, enc1));
  //pid1->setDuty(0.05);

  //mvaddstr(0, 1, "Angle: 0");
  //mvaddstr(1, 1, "Position: 0");

  // Main control loop
  while (running) {
    duration = (double)(clock() - prevTime)/CLOCKS_PER_SEC * 1000000;
    // Keyboard input
    /*
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
    */
    //clear(); // refresh the terminal window


    if (duration >= DT * std::pow(10,6)) {
        double timeSinceStart = (double)(clock() - start)/CLOCKS_PER_SEC;
        prevTime = clock();
        // Exit the control loop if no longer reading encoder.
        if (enc1->calcRotation() == -1) {
          running = 0;
          continue;
        }

        double angle = enc1->getAngle();
        angleReadings << timeSinceStart << ", " << angle << "\n";
        setpoint = sin(timeSinceStart * 2 * M_PI * 3) * 60;
        //pid1->updatePWM(setpoint, true);
    }

    if (duration >= 1.5 * DT * std::pow(10,6)) {
        cout << "overrun" << endl;
    }
    /*
    sprintf(buf, "Angle: %d", setpoint);
    mvaddstr(0, 1, buf);

    memset(buf, '\0', sizeof(char));
    sprintf(buf, "Position: %f", angle);
    mvaddstr(1, 1, buf);

    memset(buf, '\0', sizeof(char));
    sprintf(buf, "Kp: %.6g", Kp);
    mvaddstr(2, 1, buf);

    memset(buf, '\0', sizeof(char));
    sprintf(buf, "Ki: 0");
    mvaddstr(3, 1, buf);

    memset(buf, '\0', sizeof(char));
    sprintf(buf, "Kd: %.6g", Kd);
    mvaddstr(4, 1, buf);
*/
    usleep(50);
  }

  //endwin();  // restore terminal from curses

  angleReadings.close();
  delete theBus;

  return 0;
}
