# This is the Makefile to use for the NASA Snake Robot Motor Controller
# To use, at the prompt, type:
#       make testEnocder        
# or				# These will make executable driver
# 	make testPID
# or
#       make clean              # This will safely remove old stuff
CC = g++
CFLAGS = -Wall -pedantic -g
LIBS = -lncurses

ENCODER = testEncoder

PID = testPID

all: testEncoder testPID

testPID: $(PID).o
	$(CC) -o $(PID) $(CFLAGS) $(PID).o /usr/lib/librobotcontrol.so

testPID.o: $(PID).cpp ../PID.cpp ../Driver.hpp ../library/gpio.cpp ../Encoder.hpp ../library/pwm.h
	$(CC) $(CFLAGS) -c $(PID).cpp

testEncoder: $(ENCODER).o
	$(CC) -o $(ENCODER) $(CFLAGS) $(ENCODER).o 

testEncoder.o: $(ENCODER).cpp ../Encoder.cpp ../library/I2CBus.cpp
	$(CC) $(CFLAGS) -c $(ENCODER).cpp

clean:
	rm -f $(ENCODER) $(SETZERO) *.o
