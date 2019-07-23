#ifndef ENCODER_HPP
#define ENCODER_HPP

using namespace std;

class Encoder {
    private:
        int address;
        int busNum;
        double angleZero;
        double magnitudeZero;

    public:
        Encoder();
        ~Encoder();

        void init();
        void zeroAngle();
        double getAngle();
        void zeroMagnitude();
        double getMagnitude();
        double toDegree(double num);
}

#include "PID.cpp"

#endif