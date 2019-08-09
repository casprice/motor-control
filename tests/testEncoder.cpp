#include "../Encoder.hpp"
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <linux/i2c-dev.h>

using namespace std;

static int running = 0;

/**
 * Handle Ctrl-C interrupt
 */
static void __signal_handler(__attribute__ ((unused)) int dummy) {
  running = 0;
  return;
}

int main(void) {
  // Set up Ctrl-C Interrupt handling
  signal(SIGINT, __signal_handler);
  running = 1;

  Encoder* encoder = new Encoder(2, 0x40);

  // Output calculated angle
  while(running) {
    printf("\r");

    encoder->calcRotation();
    cout << "Zero: " << encoder->getZero() << " | Angle: " << encoder->getAngle() << "    ";

    fflush(stdout);
    sleep(DT); // sleep for 1 second
  }
  
  cout << endl;
}