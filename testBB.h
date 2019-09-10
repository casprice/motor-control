#include <iostream>
#include <vector>
#include <memory>
using namespace std;

#include "inih/cpp/INIReader.h"
#include "MotorControl.h"

//#define DT 0.0075
//#define DT_outter 0.015

const unsigned int enc_addr[] = { 0x40, 0x41 };
const unsigned int adc_ch[] = { 3, 5 };
static int running = 0;

// Config variables
double 	Kp, Ki, Kd;

double 	DutyCycle = -0.1;

double 	DTInner,
		DTOutter,
		MovementFrequency,
		ZeroAngleM2,
		ZeroAngleM3,
		MaxAngle;

/**
 * Handle Ctrl-C interrupt
 */
static void __signal_handler(__attribute__ ((unused)) int dummy) {
  running = 0;
  return;
}

void loadIniConfig(void) {
	INIReader reader("./config.ini");
	if (reader.ParseError() < 0) {
        throw runtime_error(string("cannot load config file."));
    }

    // Load PID
    Kp = reader.GetReal("U-JOINTS","Kp", -1.0);
    Ki = 
    Kd = 
}