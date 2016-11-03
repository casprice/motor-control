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

  I2CBus* theBus = new I2CBus(2);
  Encoder* encoder1 = new Encoder(theBus, 0x40);
  Encoder* encoder2 = new Encoder(theBus, 0x41);

  // Output calculated angle
  while(running) {
    printf("\r");

    encoder1->calcRotation();
    encoder2->calcRotation();
    cout << "Encoder 1: " << encoder1->getAngle() << " | Encoder 2: " << encoder2->getAngle() << "    ";

    fflush(stdout);
    sleep(DT); // sleep for 1 second
  }
  
  cout << endl;
}