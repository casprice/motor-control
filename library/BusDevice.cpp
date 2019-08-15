#include "BusDevice.h"

/**
 * Routine name: BusDevice(unsigned int bus, unsigned int device)
 * Description: Constructor for a generic bus device
 * Parameters: bus - the bus number
 *             device - the device number
 */
BusDevice::BusDevice(unsigned int bus, unsigned int device) {
	this->bus = bus;
	this->device = device;
	this->file=-1;
}

/**
 * Destructor is unused
 */
BusDevice::~BusDevice() {}