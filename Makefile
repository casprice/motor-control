# This is the Makefile to use for the NASA Snake Robot Motor Controller
# To use, at the prompt, type:
#       make Driver             # This will make executable driver
# or
#       make clean              # This will safely remove old stuff
CC = g++
CFLAGS = -Wall -pedantic -g
LIBS = -lncurses

TARGET = Driver

DEPENDENCIES = \
				library/BusDevice.h \
				library/BusDevice.cpp \
			  library/gpio.h \
				library/gpio.cpp \
				library/pwm.h \
				library/pwm.cpp \
			  library/I2CDevice.h \
				library/I2CDevice.cpp \
			  library/util.h \
				library/util.cpp

all: Driver

Driver:	Driver.o 
	$(CC) -o $(TARGET) $(CFLAGS) $(TARGET).o $(LIBS)

Driver.o:	Driver.cpp PID.cpp PID.hpp Encoder.cpp Encoder.hpp $(DEPENDENCIES)
	$(CC) $(CFLAGS) -c $(TARGET).cpp

clean:
	rm -f Driver Driver.o