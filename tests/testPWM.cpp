#include "../PID.hpp"
#include <iostream>
#include <signal.h>
#include <rc/motor.h>

using namespace std;

#define MOTOR1_ADC 1
#define MOTOR2_ADC 3
#define MOTOR3_ADC 5

static int running = 0;

/**
 * Handle Ctrl-C interrupt
 */
static void __signal_handler(__attribute__ ((unused)) int dummy) {
  running = 0;
  return;
}

int main(int argc, char * argv[]) {
  char *endPtr;    // Used as second param of strtol
  int motor = 1;
  double dc = 0.1;
  if (argc > 1) {
    // If too many arguments, print usage statement
    if (argc > 3) {
      cerr << "Error: Invalid number of arguments. Usage:" << endl;
      cerr << "  " << argv[0] << " [number] [duty]" << endl;
      cerr << "  number: the motor number" << endl;
      cerr << "    -- must be a valid integer" << endl;
      cerr << "    -- must be in the interval [1, 3]" << endl;
      cerr << "  duty: the duty cycle of the motor" << endl;
      cerr << "    -- must be a valid floating point number" << endl;
      cerr << "    -- must be in the interval [0.0, 1.0]" << endl;
      return -1;
    }

    // Convert first argument to number and set as motor number
    motor = strtol(argv[1], &endPtr, 10);
    if (argc > 2) {
      dc = strtod(argv[2], &endPtr);
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

    if (dc < 0.0 || dc > 1.0) {
        cerr << "Error: Duty cycle must be between 0.0 and 1.0. Aborting." << endl;
        return -1;
    }
  }
  // Set up Ctrl-C Interrupt handling
  signal(SIGINT, __signal_handler);
  running = 1;

  PID* motorCtl = new PID(motor);
  motorCtl->setDuty(dc);

  // Output calculated angle
  while(running) {
    cout << "\rSending duty cycle of " << dc << " to motor #" << motor;
    fflush(stdout);
    sleep(1); // sleep for 1 second
  }

  delete motorCtl;
  cout << endl;
}
