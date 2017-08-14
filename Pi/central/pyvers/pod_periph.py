# Using python 3

import RPi.GPIO as GPIO
import serial

"""
<code>sensor</code> is used read an input from a local hardware device.

<code>actuator</code> is used write an output from a local hardware device. It is an abstract class.

Both of these classes use a static factory method.

<code>client</code> refers to and abstract connection to the pod hardware, combining GPIO, UART and the communication with the TM4C (which uses GPIO).
"""


class Sensor:
    def __init__(self, name: str, sensor_id: int):
        self.name = name
        self.sensor_id = sensor_id
        self.value = None

    def update_sensor(self, commt: Client):
        self.value = commt.read(sensor_id=self.sensor_id)


def make_sensor(name: str):
    # TODO for different sensors
    pass


class Actuator:
    def __init__(self, name: str, actuator_id: int):
        self.name = name
        self.actuator_id = actuator_id

    def update_actuator(self, commt: Client, val: bool):
        commt.write(actuator_id=self.actuator_id, value=val)


def make_actuator(name: str):
    pass


class Client:
    def __init__(self):
        self.uart_port = serial.Serial(port="/dev/ttyACM0", baudrate=115200, parity=serial.PARITY_NONE,
                                       stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS)

        GPIO.setmode(GPIO.BCM)

        GPIO.setup(0, GPIO.OUT, initial=GPIO.LOW)
        GPIO.setup(1, GPIO.IN)

        GPIO.setup(5, GPIO.OUT, initial=GPIO.LOW)
        GPIO.setup(12, GPIO.OUT, initial=GPIO.LOW)
        GPIO.setup(13, GPIO.OUT, initial=GPIO.LOW)
        GPIO.setup(19, GPIO.OUT, initial=GPIO.LOW)
        GPIO.setup(26, GPIO.OUT, initial=GPIO.LOW)

        self.uart_port.isOpen()

    def write(self, actuator_id: int, value: bool):
        # TODO implement write method
        raise Exception("Unimplemented method")

    def read(self, sensor_id: int):
        print("Sending reset signal to TM4C.")
        GPIO.output(0, GPIO.LOW)
        print("Waiting for TM4C to reset.")
        while GPIO.input(1) != GPIO.LOW:
            pass

        print("TM4C has reset. Setting sensor id pins.")
        GPIO.output(5, GPIO.HIGH if (sensor_id & 0x1) != 0 else GPIO.LOW)
        GPIO.output(12, GPIO.HIGH if (sensor_id & 0x2) != 0 else GPIO.LOW)
        GPIO.output(13, GPIO.HIGH if (sensor_id & 0x4) != 0 else GPIO.LOW)
        GPIO.output(19, GPIO.HIGH if (sensor_id & 0x8) != 0 else GPIO.LOW)
        GPIO.output(26, GPIO.HIGH if (sensor_id & 0x10) != 0 else GPIO.LOW)

        GPIO.output(0, GPIO.HIGH)

        print("Sensor pins and reset pin set. Waiting for TM4C response.")
        self.uart_port.flush()
        while GPIO.input(1) != GPIO.HIGH:
            pass
        print("TM4C responded. Reading value from UART.")

        return int(self.uart_port.readline())

    def reset(self):
        self.read(31)
