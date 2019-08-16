#ifndef BUSDEVICE_H_
#define BUSDEVICE_H_

class BusDevice {
protected:
  unsigned int bus;   
  unsigned int device;
  int file;           
public:
  BusDevice(unsigned int bus, unsigned int device);
  ~BusDevice();
};

#include "BusDevice.cpp"

#endif /* BUSDEVICE_H_ */