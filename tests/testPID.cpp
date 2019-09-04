#include "../PID.hpp"
#include "../Driver.hpp"
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

int main(void) {
  double Kp = 0.1;
  int setpoint = 90;

  // Set up Ctrl-C Interrupt handling
  signal(SIGINT, __signal_handler);
  running = 1;

  I2CBus* theBus = new I2CBus(2);
  shared_ptr<Encoder> enc(new Encoder(theBus, 0x40));

  shared_ptr<PID> pidctl(new PID(2, Kp, 0.0, 0.0, enc));
  //pidctl->setDuty(dc);

  // Output calculated angle
  while(running) {
    cout << endl << "Encoder: " << enc->getAngle() << " | ";
    pidctl->updatePWM(setpoint, true);

    fflush(stdout);
    sleep(1); // sleep for 1 second
  }

  delete theBus;

  cout << endl;
}
