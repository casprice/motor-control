# Motor Control for the UCSD ARCLab NASA Robotic Snake 2019

---
## Overview
This repository contains the code for testing the motor control on the [Robotic Planetary Explorer](https://www.ucsdarclab.com/nasarobot) for UCSD Advanced Robotics and Controls Lab. This project is in direct collaboration with the Caltech-NASA Jet Propulsion Laboratory (Robotic Vehicles and Manipulators Group).

This project uses Beaglebone Blacks as embedded systems, each of which control 3 ESCON Module 50/5 motor drivers 

----
## Setting Up
First, give your Beaglebone Black an Internet connection by [connecting via USB](http://paulbupejr.com/beaglebone-black-internet-over-usb/). Make sure you can run 

    ping google.com

successfully. Then, clone this repository with
  
    git clone https://github.com/casprice/motor-control.git

Additionally, you must set up the device trees necessary to access the pins used to control the motor drivers. After sshing into the Beaglebone Black, enter, using the text editor of your choice: 

    sudo vim /boot/uEnv.txt

In the section titled "U-Boot Overlays", *uncomment* the first lines under "Override capes with eeprom" and edit them to say: 

    uboot_overlay_addr0=/lib/firmware/BB-PWM0-00A0.dtbo
    uboot_overlay_addr1=/lib/firmware/BB-PWM1-00A0.dtbo
    uboot_overlay_addr2=/lib/firmware/BB-PWM2-00A0.dtbo

Under "Disable auto loading of virtual capes (emmc/video/wireless/adc)", *uncomment* the following lines:
    
    disable_uboot_overlay_video=1
    disable_uboot_overlay_audio=1
    disable_uboot_overlay_wireless=1

Save and exit the file, and then run

    sudo reboot

----
## Usage
To test the program, go to the project's main directory and run 

    make

to compile the files into an executable. This program takes in the flag -m as an argument, to be followed by the number of the motor you wish to control. For example:

    ./Driver -m 2

to control motor #2.

To clean up the project directory, run

    make clean
