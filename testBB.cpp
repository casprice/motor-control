/**
 * Runs test of all motors on a joint + screw combination
 */

#include "testBB.h"

/****************************************************************
 * Main method
 ****************************************************************/
int main(int argc, char * argv[]) {
  int c;
	char *endPtr;           // Used as second param of strtol
  ofstream angleReadings; // Log readings into csv file
  angleReadings.open("angleReadings.csv");
  angleReadings << "Time (us), M2 Setpoint (deg), M2 Angle (deg), M2 Torque (Nm), \
                   M3 Setpoint (deg), M3 Angle (deg), M3 Torque (Nm)\n";
  
  // Check for -h flag
  opterr = 0;
  while ((c = getopt(argc, argv, "h")) != -1) {
    switch (c) {
      case 'h':
          __print_usage(argv);
          return -1;
      }
  }

  // Receive command-line arguments
  if (argc > 1) {
    DutyCycle = strtod(argv[1], &endPtr);
    if (argc > 2) {
      Kp = strtod(argv[2], &endPtr);
      if (argc > 3) {
        Ki = strtod(argv[3], &endPtr);
        if (argc > 4) {
          Kd = strtod(argv[4], &endPtr);
        }
      }
    }
    
    if (errno || *endPtr != '\0') {
      cerr << "Error: Invalid motor number. Aborting." << endl;
      return -1;
    }

    // Check that motor is in range
    if (DutyCycle < 0.0 || DutyCycle > 1.0) {
      cerr << "Error: Duty cycle out of range. Aborting." << endl;
      return -1;
    }
  }

  // Register keyboard interrupt
  signal(SIGINT, __signal_handler);
  running = 1;

  // Set up encoders and motor PID control
  I2CBus* theBus = new I2CBus(2);
  MotorControl* mc = new MotorControl(DTInner);

  PID* screw = new PID(1);

  for (int i = 0; i < 2; i++) {
    mc->encoder_list.push_back(shared_ptr<Encoder>(new Encoder(theBus, DTInner, enc_addr[i])));
    mc->pidctrl_list.push_back(shared_ptr<PID>(new PID(i+2, DTInner, Kp, Ki, Kd, mc->encoder_list[i])));
  }

  // Establish zero position
  mc->encoder_list[0]->setZeroPosition(ZeroAngleM2);
  mc->encoder_list[1]->setZeroPosition(ZeroAngleM3);

  // Start the motors
  cout << "setting duty cycle of screw to " << DutyCycle << endl;
  screw->setDuty(DutyCycle);
  mc->start();

  double setPoint1, setPoint2;
  auto startTime = chrono::system_clock::now();

  chrono::microseconds us_dt = chrono::microseconds(int(DTOutter * pow(10,6)));

  chrono::time_point<chrono::system_clock> prevTime;
  chrono::time_point<chrono::system_clock> endTime;

  chrono::microseconds elapsed_us;

  /*************************************************************
   * Control loop
   *************************************************************/
  while (running) {
    prevTime = chrono::system_clock::now();

    auto timeSinceStart = chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - startTime);

    setPoint1 = sin(double(timeSinceStart.count())/1000000.0 * M_PI * MovementFrequency) * Amplitude;
    setPoint2 = cos(double(timeSinceStart.count())/1000000.0 * M_PI * MovementFrequency) * Amplitude;
    //setPoint1 = 0;
    //setPoint2 = 0;

    mc->pidctrl_list[0]->setAngle(setPoint1);
    mc->pidctrl_list[1]->setAngle(setPoint2);

    angleReadings << timeSinceStart.count() << ", "
        << setPoint1 << ", "
        << mc->encoder_list[0]->getAngle() << ", "
        << mc->pidctrl_list[0]->getTorque(adc_ch[0]) << ", "
        << setPoint2 << ", "
        << mc->encoder_list[1]->getAngle() << ", "
        << mc->pidctrl_list[1]->getTorque(adc_ch[1]) << "\n";

    endTime = chrono::system_clock::now();

    //convert to microseconds
    elapsed_us = std::chrono::duration_cast<chrono::microseconds>(endTime - prevTime);

    if (elapsed_us.count() >= us_dt.count()) {
      cout << "testBB Overrun" << endl;
    } else {
      this_thread::sleep_for(us_dt - elapsed_us);
    }
  } // end control loop

  screw->setDuty(0);
  mc->stop();
  angleReadings.close();

  delete screw;
  delete mc;
  delete theBus;
}