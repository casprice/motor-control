# This is the Makefile to use for the NASA Snake Robot Motor Controller
# To use, at the prompt, type:
#       make Driver             # This will make executable driver
# or
#       make clean              # This will safely remove old stuff
CC = g++
CFLAGS = -Wall -pedantic -g -pthread
LIBS = -lncurses /usr/lib/librobotcontrol.so

TARGET = MotorControl

DEPENDENCIES = \
		MotorControl.cpp \
		MotorControl.h \
		PID.cpp \
		PID.hpp \
		Encoder.cpp \
		Encoder.hpp

LIB_DEPENDENCIES = \
		library/BusDevice.h \
		library/BusDevice.cpp \
		library/gpio.h \
		library/gpio.cpp \
		library/pwm.h \
		library/pwm.cpp \
		library/I2CBus.h \
		library/I2CBus.cpp \
		library/util.h \
		library/util.cpp

all: $(TARGET)

MotorControl:	$(TARGET).o 
	$(CC) -o $(TARGET) $(CFLAGS) $(TARGET).o $(LIBS)

MotorControl.o:	$(DEPENDENCIES) $(LIB_DEPENDENCIES)
	$(CC) $(CFLAGS) -c $(TARGET).cpp

clean:
	rm -f $(TARGET) *.o
