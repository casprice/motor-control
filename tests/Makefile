# This is the Makefile to use for the NASA Snake Robot Motor Controller
# To use, at the prompt, type:
#       make testEnocder        
# or														# These will make executable driver
# 			make testsetZeroPosition
# or
#       make clean              # This will safely remove old stuff
CC = g++
CFLAGS = -Wall -pedantic -g
LIBS = -lncurses

ENCODER = testEncoder

I2C = testI2C

all: testEncoder testsetZeroPosition

testI2C: $(I2C).o
	$(CC) -o $(I2C) $(CFLAGS) $(I2C).o

testI2C.o: $(I2C).cpp
	$(CC) $(CFLAGS) -c $(I2C).cpp

testEncoder: $(ENCODER).o
	$(CC) -o $(ENCODER) $(CFLAGS) $(ENCODER).o

testEncoder.o: $(ENCODER).cpp ../Encoder.cpp ../library/I2CDevice.cpp
	$(CC) $(CFLAGS) -c $(ENCODER).cpp

clean:
	rm -f $(ENCODER) $(SETZERO) *.o
