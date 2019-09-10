#include "testBB.h"

/**
 * Main method
 */
int main(int argc, char * argv[]) {
	char *endPtr;    // Used as second param of strtol
  double dc = -1.0;
  double Kp = 0.5;
  double Ki = 0.0;
  double Kd = 0.0;
  // Log readings into csv file
  ofstream angleReadings;
  angleReadings.open("angleReadings.csv");
  angleReadings << "Time (us), M2 Setpoint (deg), M2 Angle (deg), M2 Torque (Nm), M3 Setpoint (deg), M3 Angle (deg), M3 Torque (Nm)\n";

  // Receive command-line arguments
  if (argc == 1) {
      cerr << "Error: Invalid number of arguments. Usage:" << endl;
      cerr << "  " << argv[0] << " [dc] [Kp] [Ki] [Kd]" << endl;
      cerr << "  dc: the duty cycle" << endl;
      cerr << "    -- must be a valid floating point number" << endl;
      cerr << "    -- must be in the interval [0.0, 1.0]" << endl;
      cerr << "  Kp: the P in PID" << endl;
      cerr << "    -- must be a valid floating point number" << endl;
      cerr << "  Ki: the I in PID" << endl;
      cerr << "    -- must be a valid floating point number" << endl;
      cerr << "  Kd: the D in PID" << endl;
      cerr << "    -- must be a valid floating point number" << endl;
      return -1;
  } else {
    if (argc > 5) {
      cerr << "Error: Invalid number of arguments." << endl;
      return -1;
    }

    dc = strtod(argv[1], &endPtr);
    if (argc > 2) {
      Kp = strtod(argv[2], &endPtr);
    }
    if (argc > 3) {
      Ki = strtod(argv[3], &endPtr);
    }
    if (argc > 4) {
      Kd = strtod(argv[4], &endPtr);
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

  // Set up encoders and motor PID control
  I2CBus* theBus = new I2CBus(2);
  MotorControl* mc = new MotorControl(DT);

  PID* screw = new PID(1);

   for (int i = 0; i < 2; i++) {
     mc->encoder_list.push_back(shared_ptr<Encoder>(new Encoder(theBus, DT, enc_addr[i])));
     mc->pidctrl_list.push_back(shared_ptr<PID>(new PID(i+2, DT, Kp, Ki, Kd, mc->encoder_list[i])));
   }

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

  /*
   * Control loop
   */
  while(running) {
    prevTime = chrono::system_clock::now();


    auto timeSinceStart = chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - startTime);

    setPoint1 = sin(double(timeSinceStart.count())/1000000.0 * M_PI/4.0) * 10.0;
    setPoint2 = cos(double(timeSinceStart.count())/1000000.0 * M_PI/4.0) * 10.0;
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
  angleReadings.close();

  delete screw;
  delete mc;
  delete theBus;
}
