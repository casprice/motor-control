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
  // Specify address to read from
  this->bus->scan(address);

  // Read the registers on the bus
  return this->bus->readRegisters(reg);
}

BusDevice::~BusDevice() {}
