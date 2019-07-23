/**
 * File: Encoder.cpp
 */

#include <rc/i2c.h>

#include "Encoder.hpp"
#include "Driver.hpp"

#define ANGLMSB_REG 0xFE // angle msb
#define ANGLLSB_REG 0xFF // angle lsb
#define MAGNMSB_REG 0xFC // magnitude msb
#define MAGNLSB_REG 0xFD // magnitude lsb
#define RESOLUTION 16384.0
#define NUM_DEG 360

int address;
int busNum;
double * angleZero;
double * magnitudeZero;

// Default ctor
Encoder::Encoder() {
    address = 0x40;
    busNum = 2;
    *angleZero = 0;
    *magnitudeZero = 0;
}

// Copy ctor
Encoder::Encoder(int address, int busNum) {
    this.address = address;
    this.busNum = busNum;
    *angleZero = 0;
    *magnitudeZero = 0;
}

Encoder::~Encoder() {
    free(angleZero);
    free(magnitudeZero);
}

void Encoder::zeroAngle() {
    rc_i2c_init(busNum, address);
    rc_i2c_read_bytes(busNum, ANGLMSB_REG, 8, angleZero);
    rc_i2c_close(busNum);
}

double Encoder::getAngle() {
    double val;
    rc_i2c_init(busNum, address);
    rc_i2c_read_word(busNum, ANGLMSB_REG, &val);
    rc_i2c_close(busNum);
    return val;
}

void Encoder::zeroMagnitude() {
    rc_i2c_init(busNum, address);
    rc_i2c_read_bytes(busNum, MAGNMSB_REG, 8, angleZero);
    rc_i2c_close(busNum);
}

double Encoder::getMagnitude() {
    double val;
    rc_i2c_init(busNum, address);
    rc_i2c_read_word(busNum, MAGNMSB_REG, &val);
    rc_i2c_close(busNum);
    return val;
}

double Encoder::toDegree(double num) {
    return (num / RESOLUTION) * NUM_DEG;
}