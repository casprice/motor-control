# This is the Makefile to use for the NASA Snake Robot Motor Controller
# To use, at the prompt, type:
#       make Driver             # This will make executable driver
# or
#       make clean              # This will safely remove old stuff
CC = g++
CFLAGS = -Wall -pedantic -g
LIBS = -lncurses /usr/lib/librobotcontrol.so

TARGET = Driver

all: Driver

Driver:	Driver.o 
	$(CC) -o $(TARGET) $(CFLAGS) $(TARGET).o $(LIBS)

Driver.o:	Driver.cpp PID.cpp PID.hpp Encoder.cpp Encoder.hpp
	$(CC) $(CFLAGS) -c $(TARGET).cpp 

clean:
	rm -f *.o Driver