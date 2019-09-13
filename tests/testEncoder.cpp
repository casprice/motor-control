#include "../Encoder.hpp"
#include <iostream>
#include <signal.h>
#include <unistd.h>

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

  int addr1 = 0x40;
  int addr2 = 0x41;

  I2CBus* theBus = new I2CBus(2);
  Encoder* encoder1 = new Encoder(theBus, 0.001, addr1);
  Encoder* encoder2 = new Encoder(theBus, 0.001, addr2);
  encoder1->setZeroPosition();
  encoder2->setZeroPosition();

  // Output calculated angle
  while(running) {
    printf("\r\r");

    encoder1->calcRotation();
    encoder2->calcRotation();
    cout << hex << addr1 << ": " << encoder1->getAngle() << " | " <<
        hex << addr2 << ": " << encoder2->getAngle() << "    ";

    fflush(stdout);
    sleep(0.001); // sleep for 1 second
  }

  delete encoder1;
  delete encoder2;
  delete theBus;

  cout << endl;
}
