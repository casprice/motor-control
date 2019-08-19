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
  double dc = 0.40;

  // Set up Ctrl-C Interrupt handling
  signal(SIGINT, __signal_handler);
  running = 1;

  //shared_ptr<Encoder> enc(new Encoder);

  PID* pidctl = new PID(3, 0.08,0,0,NULL);
  pidctl->setDuty(dc);
  
  // Output calculated angle
  while(running) {
      /*
    pidctl->updatePWM(&pwm, 30);
    pidctl->updatePin(&dir, false);
    cout << "\r" << pidctl->getTorque(2);
*/
    
    fflush(stdout);
    sleep(1); // sleep for 1 second
  }

  cout << endl;
}
