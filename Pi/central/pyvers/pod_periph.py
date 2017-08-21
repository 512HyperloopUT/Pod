# Using python 3

import serial
import smbus2
import time

import RPi.GPIO as GPIO
import bno055

import quat

"""
<code>sensor</code> is used read an input from a local hardware device.

<code>actuator</code> is used write an output from a local hardware device. It is an abstract class.

Both of these classes use a static factory method.

<code>client</code> refers to and abstract connection to the pod hardware, combining GPIO, UART and the communication with the TM4C (which uses GPIO).
"""


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

        self.i2c_bus = smbus2.SMBus(1)

    def write(self, actuator_id: int, value: bool):
        # TODO implement write method
        raise Exception("Unimplemented method")

    def i2c_read(self, bus_id: int, sensor_id: int):
        if bus_id == 1:
            return self.i2c_bus.read_byte_data(sensor_id, 0)
        else:
            with smbus2.SMBusWrapper(bus_id) as bus:
                return bus.read_byte_data(sensor_id, 0)

    def analog_read(self, sensor_id: int):
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
        ResetSensor().update_sensor(self)


class Sensor:
    def __init__(self, name: str, sensor_id: int):
        self.name = name
        self.sensor_id = sensor_id
        self.value = None

    def update_sensor(self, commt: Client):
        raise Exception("Screwed up")

    def data_string(self):
        raise Exception("Screwed up")


class AnalogSensor(Sensor):
    def __init__(self, name: str, sensor_id: int):
        super().__init__(name, sensor_id)

    def update_sensor(self, commt: Client):
        self.value = commt.analog_read(sensor_id=self.sensor_id)

    def data_string(self):
        return self.name + ": " + str(self.value)


class ResetSensor(AnalogSensor):
    RESET_SENSOR = "Reset Sensor"

    def __init__(self):
        super().__init__(ResetSensor.RESET_SENSOR, 31)

    def data_string(self):
        return "Soft TM4C reset"


class I2CSensor(Sensor):
    def __init__(self, name, bus_id, sensor_id):
        super().__init__(name, sensor_id)
        self.bus_id = bus_id
        self.data = [0] * 16

    def update_sensor(self, commt: Client):
        self.value = commt.i2c_read(self.bus_id, self.sensor_id)

    def data_string(self):
        return "{0}|{1}|{2}: {3}".format(self.name, self.bus_id, self.sensor_id, self.data)


class IMUSensor(Sensor):
    def __init__(self, name: str, sensor_id: int):
        super().__init__(name, sensor_id)
        self.bno = bno055.BNO055()
        if self.bno.begin() is not True:
            print("Error initializing device")
            exit()
        time.sleep(1)
        # TODO not sure about this line
        # self.bno.setExternalCrystalUse(True)

        self.initial_rot = quat.normalize(self.bno.getQuat())
        self.value = [(0.0, 0.0, 0.0), (0.0, 0.0, 0.0), self.initial_rot, (0.0, 0.0, 0.0), time.time()]

    def update_sensor(self, commt: Client):
        self.value = self.value[0:2] + [self.bno.getQuat(), self.bno.getVector(bno055.BNO055.VECTOR_LINEARACCEL),
                                        time.time()] + self.value[2:5]

        # get the rotation from current coordinate system to original coordinate system
        rot_q = quat.q_mult(quat.q_conjugate(quat.normalize(self.value[2])), self.initial_rot)
        # Rotate relative, linear acceleration to initial orientation
        final_accel = quat.qv_mult(rot_q, self.value[3])
        # Add accel to velocity to get the current velocity
        self.value[1] = (
                self.value[1][0] + final_accel[0],
                self.value[1][1] + final_accel[1],
                self.value[1][2] + final_accel[2]
        )
        # Do a simple time delta for the new distance traveled
        self.value[0] = (
                self.value[0][0] + (self.value[1][0] * (self.value[4] - self.value[7])),
                self.value[0][1] + (self.value[1][0] * (self.value[4] - self.value[7])),
                self.value[0][2] + (self.value[1][0] * (self.value[4] - self.value[7]))
        )


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
    # TODO for different sensors
    pass


