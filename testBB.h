#ifndef TESTBB_H
#define TESTBB_H

#include <getopt.h>
#include <iostream>
#include <vector>
#include <memory>
using namespace std;

#include "./inih/cpp/INIReader.h"
#include "MotorControl.h"

const unsigned int enc_addr[] = { 0x40, 0x41 };
const unsigned int adc_ch[] = { 3, 5 };
static int running = 0;

// Config variables
double 	Kp, Ki, Kd;

double 	DutyCycle;

double 	DTInner,
		    DTOutter,
		    MovementFrequency,
		    ZeroAngleM2,
		    ZeroAngleM3,
		    Amplitude;

/**
 * Handle Ctrl-C interrupt
 */
static void __signal_handler(__attribute__ ((unused)) int dummy) {
  running = 0;
  return;
}

// printed if some invalid argument was given
static void __print_usage(char * argv[]) {
  cerr << "Usage:" << endl;
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
}

/**
 * Load values from config.ini
 */
void loadIniConfig(void) {
	INIReader reader("./config.ini");
	if (reader.ParseError() < 0) {
    throw runtime_error(string("cannot load config file."));
  }

  // Load real (floating point double) values
  Kp = reader.GetReal("U-JOINTS", "Kp", -1);
  Ki = reader.GetReal("U-JOINTS", "Ki", -1);
  Kd = reader.GetReal("U-JOINTS", "Kd", -1);

  DutyCycle = reader.GetReal("SCREW", "DutyCycle", -1);

  DTInner = reader.GetReal("CONTROL", "DTInner", -1);
  DTOutter = reader.GetReal("CONTROL", "DTOutter", -1);
  MovementFrequency = reader.GetReal("CONTROL", "MovementFrequency", -1);
  ZeroAngleM2 = reader.GetReal("CONTROL", "ZeroAngleM2", -1);
  ZeroAngleM3 = reader.GetReal("CONTROL", "ZeroAngleM3", -1);
  Amplitude = reader.GetReal("CONTROL", "Amplitude", -1);
}

#endif //TESTBB_H