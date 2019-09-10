#include "MotorControl.h"
#include <iostream>
#include <vector>
#include <memory>
using namespace std;

#define DT 0.005
#define DT_outter 0.005

const unsigned int enc_addr[] = { 0x40, 0x41 };
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
  int motor = 0;   // command line arg
  double Kp = 0.5;
  double Kd = 0.0;
  int setpoint = 0;   // angle we want motor to spin to
  char buf[20];    // buffer to print angle and position vals

  // Receive command-line arguments
  if (argc == 1) {
      cerr << "Error: Invalid number of arguments. Usage:" << endl;
      cerr << "  " << argv[0] << " [number] [Kp] [Ki]" << endl;
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

  // Initialize encoder and PID lists
	// vector<shared_ptr<Encoder>> encoder_list;
	// encoder_list.resize(2);
	// vector<shared_ptr<PID>> pidctrl_list;
	// pidctrl_list.resize(3);

  I2CBus* theBus = new I2CBus(2);
  MotorControl* mc = new MotorControl(DT);

	// shared_ptr<PID>(new PID(1, Kp, 0.0, Kd, NULL));

  // for (int i = 0; i < 2; i++) {
  //   mc->encoder_list.push_back(shared_ptr<Encoder>(new Encoder(theBus, DT, enc_addr[i], 16384.0)));
  //   mc->pidctrl_list.push_back(shared_ptr<PID>(new PID(i+2, DT, Kp, 0.0, Kd, mc->encoder_list[i])));
  // }

    mc->encoder_list.push_back(shared_ptr<Encoder>(new Encoder(theBus, DT, enc_addr[motor-2])));
    mc->pidctrl_list.push_back(shared_ptr<PID>(new PID(motor, DT, Kp, 0.0, Kd, mc->encoder_list[0])));


  mc->start();

  double setPoint = 0;
  auto startTime = chrono::system_clock::now();

  int ms_dt = DT_outter*pow(10,3);

  while(running) {
    auto timeSinceStart = chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - startTime);

    //setPoint = sin(double(timeSinceStart.count())/1000000 * M_PI) * 45.0;
    setPoint = 0;
    //cout << "Encoder 1: " << mc->encoder_list[0]->getAngle() << endl;

    mc->pidctrl_list[0]->setAngle(setPoint);
    this_thread::sleep_for(chrono::milliseconds(ms_dt));
  }

  mc->stop();

  delete theBus;
  delete mc;
}
