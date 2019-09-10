# This is the Makefile to use for the NASA Snake Robot Motor Controller
# To use, at the prompt, type:
#       make testEnocder        
# or				# These will make executable driver
# 	make testPWM
# or
#       make clean              # This will safely remove old stuff
CC = g++
CFLAGS = -Wall -pedantic -g
LIBS = -lncurses

ENCODER = testEncoder

PWM = testPWM

TORQUE = testTorque

all: testEncoder testPWM testTorque

testPWM: $(PWM).o
	$(CC) -o $(PWM) $(CFLAGS) $(PWM).o /usr/lib/librobotcontrol.so

testPWM.o: $(PWM).cpp ../PID.cpp ../Driver.hpp ../library/gpio.cpp ../Encoder.hpp ../library/pwm.cpp
	$(CC) $(CFLAGS) -c $(PWM).cpp

testEncoder: $(ENCODER).o
	$(CC) -o $(ENCODER) $(CFLAGS) $(ENCODER).o 

testEncoder.o: $(ENCODER).cpp ../Encoder.cpp ../library/I2CBus.cpp
	$(CC) $(CFLAGS) -c $(ENCODER).cpp

testTorque: $(TORQUE).o
	$(CC) -o $(TORQUE) $(CFLAGS) $(TORQUE).o /usr/lib/librobotcontrol.so

testTorque.o: $(TORQUE).cpp ../PID.cpp ../Encoder.cpp
	$(CC) $(CFLAGS) -c $(TORQUE).cpp

clean:
	rm -f $(ENCODER) $(PWM) $(TORQUE) *.o
