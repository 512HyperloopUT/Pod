from RPi import GPIO
from enum import Enum
import serial


class WriteDir(Enum):
    REVERSE = 2
    NEUTRAL = 0
    FORWARD = 1


class CommPort:
    def __init__(self):
        self.ser = serial.Serial(port="/dev/ttyACM0", baudrate=115200, parity=serial.PARITY_NONE,
                                 stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS)
        self.ser.isOpen()

        GPIO.setmode(GPIO.BCM)

        GPIO.setup(0, GPIO.OUT, initial=GPIO.LOW)  # start cmd
        GPIO.setup(23, GPIO.OUT, initial=GPIO.LOW)  # cmd type
        GPIO.setup(24, GPIO.OUT, initial=GPIO.LOW)  # write dir 0
        GPIO.setup(25, GPIO.OUT, initial=GPIO.LOW)  # write dir 1
        GPIO.setup(1, GPIO.IN)  # finish cmd

        GPIO.setup(5, GPIO.OUT, initial=GPIO.LOW)  # id pin 0
        GPIO.setup(12, GPIO.OUT, initial=GPIO.LOW)  # id pin 1
        GPIO.setup(13, GPIO.OUT, initial=GPIO.LOW)  # id pin 2
        GPIO.setup(19, GPIO.OUT, initial=GPIO.LOW)  # id pin 3
        GPIO.setup(26, GPIO.OUT, initial=GPIO.LOW)  # id pin 4
        print("comms initialized")

    def __del__(self):
        self.ser.close()
        GPIO.cleanup()
        print("freed comm resources")

    def read(self, id):
        self.__reset()
        self.__set_type(False)
        self.__write_id(id)
        self.ser.flush()
        self.__finish()
        return int(self.ser.readline())

    def write(self, id, dir):
        self.__reset()
        self.__set_type(True)
        self.__write_id(id)
        self.__write_dir(dir)
        self.__finish()

    def __reset(self):
        GPIO.output(0, GPIO.LOW)
        while GPIO.input(1) != GPIO.LOW:
            pass

    def __set_type(self, type):
        GPIO.output(23, GPIO.HIGH if type else GPIO.LOW)

    def __write_id(self, id):
        GPIO.output(5, GPIO.HIGH if (id & 0x1) != 0 else GPIO.LOW)
        GPIO.output(12, GPIO.HIGH if (id & 0x2) != 0 else GPIO.LOW)
        GPIO.output(13, GPIO.HIGH if (id & 0x4) != 0 else GPIO.LOW)
        GPIO.output(19, GPIO.HIGH if (id & 0x8) != 0 else GPIO.LOW)
        GPIO.output(26, GPIO.HIGH if (id & 0x10) != 0 else GPIO.LOW)

    def __write_dir(self, dir):
        if dir == WriteDir.REVERSE:
            GPIO.output(24, GPIO.HIGH)
            GPIO.output(25, GPIO.LOW)
        elif dir == WriteDir.FORWARD:
            GPIO.output(24, GPIO.LOW)
            GPIO.output(25, GPIO.HIGH)
        else:
            GPIO.output(24, GPIO.LOW)
            GPIO.output(25, GPIO.LOW)

    def __finish(self):
        GPIO.output(0, GPIO.HIGH)
        while GPIO.input(1) != GPIO.HIGH:
            pass
