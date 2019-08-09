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
  double finalTorque = 0; 

  // Set up Ctrl-C Interrupt handling
  signal(SIGINT, __signal_handler);
  running = 1;

  // GPIO/PWM setup
  PWM pwm(30000);
  //pwm.setPeriod(100000);
  //pwm.setDutyCycle(50.0);
  GPIO dir(M2_DIR);
  GPIO enable(M2_ENABLE);

  //shared_ptr<Encoder> enc(new Encoder);

  PID* pidctl = new PID(2,0,0,NULL);
  
  
  // Output calculated angle
  while(running) {
    
    //pidctl->updatePWM(&pwm, 30);
    //pidctl->updatePin(&dir, false);
    finalTorque = pidctl->getTorque(MOTOR3_ADC);

    cout << "\r" << finalTorque;

    fflush(stdout);
    sleep(0.1); // sleep for 1 second
  }
  
  cout << "\rFinal torque: " << finalTorque;
  
  cout << endl;
}