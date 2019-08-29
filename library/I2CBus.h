#ifndef I2C_H_
#define I2C_H_

#define BBB_I2C_0 "/dev/i2c-0"
#define BBB_I2C_1 "/dev/i2c-1"
#define BBB_I2C_2 "/dev/i2c-2"

/**
 * busTracker class
 */
class busTracker {
  protected:
    static busTracker *theTracker;

    // Protected constructor to prevent public call
    busTracker() {}

  public:
    /**
     * Routine name: getInstance()
     * Description: Creates an instance of an I2C bus tracker if one does not
     *              already exist, otherwise returns the existing tracker.
     * Parameters: a_bus - The number of the bus to be opened.
     *             a_address - The address of the device.
     * Return value: A pointer to the static instance of the bus tracker.
     */
    static busTracker* getInstance() {
      if(!theTracker) {
        theTracker = new busTracker;
      }
      return theTracker;
    }

    static void deleteTracker() {
      delete theTracker;
    }

    ~busTracker() {}
};

/**
 * I2CBus class
 */
class I2CBus {
  protected:
    unsigned int bus;     // the bus number
    int file;             // the file handle to the device

  public:
    I2CBus(unsigned int a_bus);
    int openDevice(void);
    int scan(unsigned int addr);
    int writeRegister(unsigned char value);
    unsigned char* readRegisters(unsigned int fromAddress);
    void closeDevice(void);
    ~I2CBus(void);
};

#include "I2CBus.cpp"

#endif /* I2C_H_ */
