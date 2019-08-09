#include "../PID.hpp"
#include "../Driver.hpp"
#include <iostream>
#include <signal.h>

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
  double maxTorque = 0; 

  // Set up Ctrl-C Interrupt handling
  signal(SIGINT, __signal_handler);
  running = 1;

  // GPIO/PWM setup
  PWM pwm(20000);
  GPIO dir(M2_DIR);
  GPIO enable(M2_ENABLE);
  //GPIO pwm_pin(PWM_PIN);

  //shared_ptr<Encoder> enc(new Encoder);

  PID* pidctl = new PID(2,0,0,NULL);
  pwm.setDutyCycle(1.0);
  
  // Output calculated angle
  while(running) {
    
    //pidctl->updatePWM(&pwm, 30);
    //pidctl->updatePin(&dir, false);
    maxTorque = pidctl->getCurrent(MOTOR3_ADC);

    cout << "\r" << maxTorque;

    fflush(stdout);
    sleep(0.1); // sleep for 1 second
  }
  
  cout << "\r" << maxTorque;

  cout << endl;
}