/**
 * File: Encoder.hpp
 */

#ifndef ENCODER_HPP
#define ENCODER_HPP

class Encoder {
    private:
        int address;
        int busNum;
        double angleZero;
        double magnitudeZero;

    public:
        Encoder();
        Encoder(int address, int busNum);
        ~Encoder();

        void zeroAngle();
        double getAngle();
        void zeroMagnitude();
        double getMagnitude();
        double toDegree(double num);
}

#include "PID.cpp"

#endif