#include "MotorControl.h"
#include <iostream>
#include <vector>
#include <memory>
using namespace std;

#define DT 0.0075
#define DT_outter 0.015

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
    double dc = -1.0;
  double Kp = 0.5;
  double Kd = 0.0;
  int setpoint = 0;   // angle we want motor to spin to
  char buf[20];    // buffer to print angle and position vals

  // Receive command-line arguments
  if (argc == 1) {
      cerr << "Error: Invalid number of arguments. Usage:" << endl;
      cerr << "  " << argv[0] << " [number] [Kp] [Ki]" << endl;
      cerr << "  number: the duty cycle" << endl;
      cerr << "    -- must be a valid floating point number" << endl;
      cerr << "    -- must be in the interval [0.0, 1.0]" << endl;
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

    // Convert first argument to number and set as duty cycle for screw
    dc = strtod(argv[1], &endPtr);
    if (argc > 2) {
        Kp = strtod(argv[2], &endPtr);
    }
    if (argc > 3) {
        Kd = strtod(argv[3], &endPtr);
    }
    if (errno || *endPtr != '\0') {
      cerr << "Error: Invalid motor number. Aborting." << endl;
      return -1;
    }

    // Check that motor is in range
    if (dc < 0.0 || dc > 1.0) {
      cerr << "Error: Duty cycle out of range. Aborting." << endl;
      return -1;
    }
  }

  // Register keyboard interrupt
  signal(SIGINT, __signal_handler);
  running = 1;

  I2CBus* theBus = new I2CBus(2);
  MotorControl* mc = new MotorControl(DT);

  PID* screw = new PID(1);

   for (int i = 0; i < 2; i++) {
     mc->encoder_list.push_back(shared_ptr<Encoder>(new Encoder(theBus, DT, enc_addr[i])));
     mc->pidctrl_list.push_back(shared_ptr<PID>(new PID(i+2, DT, Kp, 0.0, Kd, mc->encoder_list[i])));
   }

    //mc->encoder_list.push_back(shared_ptr<Encoder>(new Encoder(theBus, DT, enc_addr[motor-2])));
    //mc->pidctrl_list.push_back(shared_ptr<PID>(new PID(motor, DT, Kp, 0.0, Kd, mc->encoder_list[0])));

   // Start the motors
   cout << "setting duty cycle of screw to " << dc << endl;
  screw->setDuty(dc);
  mc->start();

  double setPoint1 = 0;
  double setPoint2 = 0;
  auto startTime = chrono::system_clock::now();

  chrono::microseconds us_dt = chrono::microseconds(int(DT_outter*pow(10,6)));

  chrono::time_point<chrono::system_clock> prevTime;
  chrono::time_point<chrono::system_clock> endTime;

  chrono::microseconds elapsed_us;

  while(running) {
    prevTime = chrono::system_clock::now();
    
    auto timeSinceStart = chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - startTime);

    setPoint1 = sin(double(timeSinceStart.count())/1000000.0 * M_PI - M_PI/2.0) * 25.0;
    setPoint2 = sin(double(timeSinceStart.count())/1000000.0 * M_PI ) * 25.0;

    mc->pidctrl_list[0]->setAngle(setPoint1);
    mc->pidctrl_list[1]->setAngle(setPoint2);

    endTime = chrono::system_clock::now();

    //convert to ms
    elapsed_us = std::chrono::duration_cast<chrono::microseconds>(endTime - prevTime);

    if (elapsed_us.count() >= us_dt.count()) {
      cout << "testBB Overrun" << endl;
    }
    else{
      this_thread::sleep_for(us_dt - elapsed_us);
    }
  }

  screw->setDuty(0);
  mc->stop();

  delete screw;
  delete theBus;
  delete mc;
}
