# Using python 3

import serial
import smbus2
import time
import RPi.GPIO as GPIO
import bno055
import quat
import hyper_comms as comms

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

class Actuator:
    def __init__(self, name: str, actuator_id: int):
        self.name = name
        self.actuator_id = actuator_id

    def update_actuator(self, commt: Client, val: bool):
        commt.write(actuator_id=self.actuator_id, value=val)
