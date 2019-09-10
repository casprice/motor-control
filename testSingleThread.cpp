#include <chrono>
#include <iostream>
#include <memory>
#include <signal.h>
#include <thread>
#include <vector>

using namespace std;

#include "Encoder.hpp"
#include "PID.hpp"

#define DT 0.004

const unsigned int enc_addr[] = { 0x40, 0x41 };
const unsigned int adc_ch[] = { 3, 5 };
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
  ofstream m2_readings;
  ofstream m3_readings;
  ofstream angleReading_list[2] = { m2_readings, m3_readings };
  m2_readings.open("m2_readings.csv");
  m3_readings.open("m3_readings.csv");

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
  vector<shared_ptr<Encoder>> encoder_list;
  vector<shared_ptr<PID>> pidctrl_list;
  PID* screw = new PID(1);

  for (int i = 0; i < 2; i++) {
    encoder_list.push_back(shared_ptr<Encoder>(new Encoder(theBus, DT, enc_addr[i])));
    pidctrl_list.push_back(shared_ptr<PID>(new PID(i+2, DT, Kp, 0.0, Kd, encoder_list[i])));
  }

  // Start the motors
  cout << "setting duty cycle of screw to " << dc << endl;
  screw->setDuty(dc);
  for (int i = 0; i < pidctrl_list.size(); i++) {
    encoder_list[i]->calcRotation();
    pidctrl_list[i]->setAngle(encoder_list[i]->getAngle());
  }

  double setPoint_list[2] = { 0, 0 };
  auto startTime = chrono::system_clock::now();

  chrono::microseconds us_dt = chrono::microseconds(int(DT*pow(10,6)));

  chrono::time_point<chrono::system_clock> prevTime;
  chrono::time_point<chrono::system_clock> endTime;

  chrono::microseconds elapsed_us;

  while(running) {
    prevTime = chrono::system_clock::now();

    auto timeSinceStart = chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - startTime);

    setPoint_list[0] = sin(double(timeSinceStart.count())/1000000.0 * M_PI - M_PI/2.0) * 25.0;
    setPoint_list[1] = sin(double(timeSinceStart.count())/1000000.0 * M_PI ) * 25.0;

    for (int i = 0; i < pidctrl_list.size(); i++) {
      if (encoder_list[i]->calcRotation() == -1) {
        running = 0;
        continue;
      }
      pidctrl_list[i]->setAngle(setPoint_list[i]);
      pidctrl_list[i]->updatePWM(true);
      angleReading_list[i] << timeSinceStart.count() << ", " 
              << encoder_list[i]->getAngle() << ", " 
              << pidctrl_list[i]->getTorque(adc_ch[i]) << "\n";
    }

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
  for (int i = 0; i < pidctrl_list.size(); i++) {
    encoder_list[i]->calcRotation();
    pidctrl_list[i]->setAngle(encoder_list[i]->getAngle());
    pidctrl_list[i]->setDuty(0);
  }

  delete screw;
  delete theBus;
  m2_readings.close();
  m3_readings.close();
}
