#ifndef BUSDEVICE_H_
#define BUSDEVICE_H_

class BusDevice {
protected:
  I2CBus* bus;
  unsigned int address;
public:
  BusDevice(I2CBus* a_bus, unsigned int a_address);
  unsigned char * readRegisters(unsigned int reg);
  ~BusDevice();
};

#include "BusDevice.cpp"

#endif /* BUSDEVICE_H_ */