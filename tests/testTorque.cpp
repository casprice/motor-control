#include "../PID.hpp"
#include "../Encoder.hpp"
#include <iostream>
#include <memory>
#include <signal.h>
#include <vector>
#include <rc/motor.h>

#define DT 0.005
#define TORQUE_CONST 6.18 // mNm/A
#define GEAR_RATIO 44     // 44:1
#define GEARBOX_EFF 81    // 81% efficiency

using namespace std;

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
  // Set up Ctrl-C Interrupt handling
  signal(SIGINT, __signal_handler);
  running = 1;

  // initialize hardware first
  if(rc_adc_init()){
    cerr << "ERROR: failed to run rc_init_adc()" << endl;
    return -1;
  }

  double torque2, torque3;

  // Output calculated angle
  while(running) {
    //torque2 = (rc_adc_read_volt(adc_ch[0]) * 1681/681 - 2) * 3.4/2 * (TORQUE_CONST/1000) * GEAR_RATIO * (GEARBOX_EFF/100);
    //torque3 = (rc_adc_read_volt(adc_ch[1]) * 1681/681 - 2) * 3.4/2 * (TORQUE_CONST/1000) * GEAR_RATIO * (GEARBOX_EFF/100);
    torque2 = (rc_adc_read_volt(adc_ch[0]) * 1681.0/681.0 - 2.0) * 3.4/2.0 * (TORQUE_CONST/1000.0) * GEAR_RATIO * (GEARBOX_EFF/100.0);
    torque3 = (rc_adc_read_volt(adc_ch[1]) * 1681.0/681.0 - 2.0) * 3.4/2.0 * (TORQUE_CONST/1000.0) * GEAR_RATIO * (GEARBOX_EFF/100.0);
    printf("\rMotor 2: %8.6f", torque2);
    printf("  |  ");
    printf("Motor 3: %8.6f", torque3);

    fflush(stdout);
    sleep(0.001);
  }

  cout << endl;

  return 0;
}
