#include "../Encoder.hpp"
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <linux/i2c-dev.h>

#include "../inih/cpp/INIReader.h"

#define ZEROM1 

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

  INIReader reader("../config.ini");
	if (reader.ParseError() < 0) {
    throw runtime_error(string("cannot load config file."));
  }

  int addr1 = 0x40;
  int addr2 = 0x41;

  double ZeroAngleM2 = reader.GetReal("CONTROL", "ZeroAngleM2", -1);
  double ZeroAngleM3 = reader.GetReal("CONTROL", "ZeroAngleM3", -1);

  I2CBus* theBus = new I2CBus(2);
  Encoder* encoder1 = new Encoder(theBus, 0.001, addr1);
  Encoder* encoder2 = new Encoder(theBus, 0.001, addr2);
  encoder1->setZeroPosition(ZeroAngleM2);
  encoder2->setZeroPosition(ZeroAngleM3);

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
