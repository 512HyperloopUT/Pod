import RPi.GPIO as GPIO
from enum import Enum
import serial

ser = serial.Serial(port="/dev/ttyACM0", baudrate=115200, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS)

ser.isOpen()

print("serial initialized")

class WriteDir(Enum):
    REVERSE = 2
    NEUTRAL = 0
    FORWARD = 1

def init():
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BCM)

    GPIO.setup(0, GPIO.OUT, initial=GPIO.LOW)#start cmd
    GPIO.setup(23, GPIO.OUT, initial=GPIO.LOW)#cmd type
    GPIO.setup(24, GPIO.OUT, initial=GPIO.LOW)#write dir 0
    GPIO.setup(25, GPIO.OUT, initial=GPIO.LOW)#write dir 1
    GPIO.setup(1, GPIO.IN)#finish cmd

    GPIO.setup(5, GPIO.OUT, initial=GPIO.LOW)#id pin 0
    GPIO.setup(12, GPIO.OUT, initial=GPIO.LOW)#id pin 1
    GPIO.setup(13, GPIO.OUT, initial=GPIO.LOW)#id pin 2
    GPIO.setup(19, GPIO.OUT, initial=GPIO.LOW)#id pin 3
    GPIO.setup(26, GPIO.OUT, initial=GPIO.LOW)#id pin 4
    print("gpio initialized")

def read(id):
    __reset()
    __set_type(False)
    __write_id(id)
    __finish()
    ser.flush()
    return int(ser.readline())

def write(id, dir):
    __reset()
    __set_type(True)
    __write_id(id)
    __write_dir(dir)
    __finish()

def free():
    ser.close()
    GPIO.cleanup()
    print("freed comm resources")

def __reset():
    GPIO.output(0, GPIO.LOW)
    while GPIO.input(1) != GPIO.LOW:
        pass

def __set_type(type):
    GPIO.output(23, GPIO.HIGH if type else GPIO.LOW)

def __write_id(id):
    GPIO.output(5, GPIO.HIGH if (id & 0x1) != 0 else GPIO.LOW)
    GPIO.output(12, GPIO.HIGH if (id & 0x2) != 0 else GPIO.LOW)
    GPIO.output(13, GPIO.HIGH if (id & 0x4) != 0 else GPIO.LOW)
    GPIO.output(19, GPIO.HIGH if (id & 0x8) != 0 else GPIO.LOW)
    GPIO.output(26, GPIO.HIGH if (id & 0x10) != 0 else GPIO.LOW)

def __write_dir(dir):
    if dir == WriteDir.REVERSE:
        GPIO.output(24, GPIO.HIGH)
        GPIO.output(25, GPIO.LOW)
    elif dir == WriteDir.FORWARD:
        GPIO.output(24, GPIO.LOW)
        GPIO.output(25, GPIO.HIGH)
    else:
        GPIO.output(24, GPIO.LOW)
        GPIO.output(25, GPIO.LOW)

def __finish():
    GPIO.output(0, GPIO.HIGH)
    while GPIO.input(1) != GPIO.HIGH:
        pass
