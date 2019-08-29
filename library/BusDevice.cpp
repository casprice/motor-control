#include "BusDevice.h"

/**
 * Routine name: BusDevice(unsigned int bus, unsigned int device)
 * Description: Constructor for a generic bus device
 * Parameters: bus - the bus number
 *             device - the device number
 */
BusDevice::BusDevice(I2CBus* a_bus, unsigned int a_address) {
  this->bus = a_bus;
  this->address = a_address;
}

unsigned char * BusDevice::readRegisters(unsigned int reg) {
  return this->bus->readRegisters(reg);
}

/**
 * Destructor is unused
 */
BusDevice::~BusDevice() {}
