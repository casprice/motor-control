#include "../Encoder.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;

/**
 * 
 */
int main(void) {
  Encoder encoder;
  encoder.setZeroPosition();
  encoder.calcRotation();
  cout << "Angle: " << encoder.getAngle();

  cout << endl;
}