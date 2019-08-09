#!/usr/bin/env python2

# build parameters
#build_params = ['local_test']
build_params = ['bbb_test']

#pin_map = {
#	"pwm": "P8_13",
#	"enable": "P8_11",
#	"direction": "P8_10"
#}

pin_map = {
	"pwm": "P8_19",
	"enable": "P8_10",
	"direction": "P8_18"
}

# include
import curses
import signal
import sys
import math
from time import sleep

# conditional includes
if 'bbb_test' in build_params:
    from smbus import SMBus
    import Adafruit_BBIO.PWM as PWM
    import Adafruit_BBIO.GPIO as GPIO

# register defintions
AS5048B_registers = {
    "ANGLMSB_REG":0xFE, # angle msb
    "ANGLLSB_REG":0xFF, # angle lsb
    "MAGNMSB_REG":0xFC, # magnitude msb
    "MAGNLSB_REG":0xFD, # magnitude lsb
}

def truncate(number, digits):
    stepper = 10.0 ** digits
    return math.trunc(stepper * number) / stepper

def clip(a_number, a_min, a_max):
    if a_number < a_min:
        a_number = a_min
    if a_number > a_max:
        a_number = a_max
    return a_number

def shutdown(sig, frame):
    if 'bbb_test' in build_params:
        GPIO.output(pin_map["enable"], GPIO.LOW)
        PWM.stop(pin_map["pwm"])
        PWM.cleanup()
        GPIO.cleanup()
    curses.nocbreak()
    curses.echo()
    curses.endwin()
    sys.exit(0)

class pid_ctl_dir_en:
    def __init__(self, p, i=0, d=0):
        self.p = p
        self.i = i
        self.d = d
        self.total_error = 0
        self.previous_error = 0
    def update(self, current, goal, dir_inverted=False):
        #adjusted_duty = (self.p*(goal-current)) + (self.i*self.total_error) + (self.d*self.previous_error)
        adjusted_duty = 5
        if 'bbb_test' in build_params:
            # direction
            if ((adjusted_duty < 0) != dir_inverted):
                GPIO.output(pin_map["direction"], GPIO.HIGH)
            else:
                GPIO.output(pin_map["direction"], GPIO.LOW)
            # enable
            PWM.set_duty_cycle(pin_map["pwm"], clip(abs(adjusted_duty), 0, 100))
        self.total_error += goal - current
        self.previous_error = goal - current
    def clear_i(self):
        self.total_error = 0
class AS5048B:
    def __init__(self, address=0x40, busnum=2):
        self.address = address
        self.busnum = busnum
        #print(self.device.errMsg())
        self.angle_zero = 0
        self.magnitude_zero = 0
    def test(self):
        pass
    def zero_angle(self):
        bus = SMBus(self.busnum)
        self.angle_zero = bus.read_i2c_block_data(self.address, AS5048B_registers["ANGLMSB_REG"], 8)
        bus.close()
    def getAngle(self):
        bus = SMBus(self.busnum)
        #return self.bus.read_i2c_block_data(self.address, AS5048B_registers["ANGLMSB_REG"], 8) - self.angle_zero
        val = (bus.read_i2c_block_data(self.address, AS5048B_registers["ANGLMSB_REG"],2))
        bus.close()
        return val
    def zero_magnitude(self):
        bus = SMBus(self.busnum)
        self.magnitude_zero = bus.read_i2c_block_data(self.address, AS5048B_registers["MAGNMSB_REG"], 8)
        bus.close()
    def getMagnitude(self):
        #return self.bus.read_i2c_block_data(self.address, AS5048B_registers["MAGNMSB_REG"], 16) - self.magnitude_zero
        pass
    def toDegree(self, num):
        resolution = 16384.0
        return (num / resolution ) * 360

def main():
    # register ctrl-c interrupt capture
    signal.signal(signal.SIGINT, shutdown)
    # start curses
    stdscr = curses.initscr() # inititalize curses
    curses.noecho() # don't echo input to the screen
    curses.cbreak() # react to keys instantly w/o Enter
    stdscr.nodelay(True)
    stdscr.keypad(True)
    value = 0
    counter = 0

     # setup GPIO/pwm (hardware)
    if 'bbb_test' in build_params:
        GPIO.setup(pin_map["direction"], GPIO.OUT)
        GPIO.setup(pin_map["enable"], GPIO.OUT)
        GPIO.output(pin_map["enable"], GPIO.HIGH)
        # set frequency of PWM to 20khz
        # pwm.start(channel, duty, frequency, polarity=0)
        PWM.start(pin_map["pwm"], 0, 20000) 

    # pid setup (Kp, Kd, Ki)
    pid_ctl = pid_ctl_dir_en(0.8, 0.0, 0.0)
    i2c_device = AS5048B()
    # loop
    step = 1
    while True:
        # control
        c = stdscr.getch()
        if c == curses.KEY_LEFT:
            value -= step
        elif c == curses.KEY_RIGHT:
            value += step
        elif c == ord(' '):
            if(step == 10):
                step = 1
            else:
                step = 10

        value = clip(value, -175,175)
        stdscr.clear()
        stdscr.addstr(0,1,"Angle: {}".format(value))

        temp_list = 8192
        #temp_list = (i2c_device.getAngle()[0] << 6) + (i2c_device.getAngle()[1] & 0x3F)
        
        stdscr.addstr(1,1, "Postion: {}".format(truncate(i2c_device.toDegree(temp_list)-180, 3)))
        # pid section
        pid_ctl.update(current=i2c_device.toDegree(temp_list) - 180, goal=value, dir_inverted=False)

        counter += 1
        if (counter % 1000) == 0:
            pid_ctl.clear_i()
        sleep(0.001)
    GPIO.output(pin_map["enable"], GPIO.LOW)
if __name__ == '__main__':
    main()
