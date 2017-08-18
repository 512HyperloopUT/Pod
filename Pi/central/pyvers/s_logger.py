#!/usr/bin/env python3
# Using python 3

# set up i2c on the pi
# first, un-blacklist i2c
# next, add the i2c modules to the loaded kernel modules
# afterthat, install i2c-tools and reboot
# finally, run i2cdetect

# install smbus2

# change i2c baudrate to 50kHz, if using i2c

import time
import Rpi.GPIO

import pod_periph
import bno055


class IMUSensor(pod_periph.Sensor):
    def __init__(self, name: str, sensor_id: int):
        super().__init__(name, sensor_id)
        self.bno = bno055.BNO055()
        if self.bno.begin() is not True:
            print("Error initializing device")
            exit()
        time.sleep(1)
        self.bno.setExternalCrystalUse(True)

    def update_sensor(self, commt: pod_periph.Client):
        super(commt)
        self.value = self.value[0:2] + [self.bno.getQuat(), self.bno.getVector(bno055.BNO055.VECTOR_LINEARACCEL), time.time()] + self.value[2:5]
        # TODO modify velocity (self.value[1])
        tuple(map(sum, zip(self.value[1], self.value[4])))
        # TODO modify distance (self.value[0])
        self.value[0] = self.value[0] + (self.value[1] * (self.value[3] - self.value[7]))


# Emergency brake
EBRAKE_ACTU_L_FORW = 0
EBRAKE_ACTU_L_BACK = 0
EBRAKE_ACTU_R_FORW = 0
EBRAKE_ACTU_R_BACK = 0
EBRAKE_POTENT_L = 0
EBRAKE_POTENT_R = 0
EBRAKE_EMAG_L = 0
EBRAKE_EMAG_R = 0

# Main brake
MAINB_ACTU_L_FORW = 0
MAINB_ACTU_L_BACK = 0
MAINB_ACTU_R_FORW = 0
MAINB_ACTU_R_BACK = 0

# Actuators
ACTU_FORW = []
ACTU_BACK = []

# Record values
PROX_LF = 0
PROX_RF = 0

# Report on velocity + acceleration
# Calc distance traveled
IMU_BUS = 1
IMU_CH = 0x28

# Actuators
EXT_FREE = 0
RET_FREE = 1
EXT_TIME = 2
RET_TIME = 3
EXT_DIST = 4
RET_DIST = 5
EXT_TARG = 6
RET_TARG = 7
HALT = 8

# Electromagnet
ON = 0
OFF = 1

# Target states
EBRAKE_ACTU_L_STATE = [HALT, None]
EBRAKE_ACTU_R_STATE = [HALT, None]
MAINB_ACTU_L_STATE = [HALT, None]
MAINB_ACTU_R_STATE = [HALT, None]
EBRAKE_EMAG_L_STATE = [OFF]
EBRAKE_EMAG_R_STATE = [OFF]

ACTU_STATE = []  # 2d list

cli = pod_periph.Client()

l_potentiometer = pod_periph.AnalogSensor("emergencypotent_left", EBRAKE_POTENT_L)
r_potentiometer = pod_periph.AnalogSensor("emergencypotent_right", EBRAKE_POTENT_R)

sensor_ports = [
    IMUSensor("imu", IMU_CH),
    pod_periph.AnalogSensor("prox0", PROX_LF),
    pod_periph.AnalogSensor("prox1", PROX_RF),
    l_potentiometer,
    r_potentiometer
]


def update_sensors():
    for sensor in sensor_ports:
        sensor.update_sensor(cli)


def log(outfile):
    for sensor in sensor_ports:
        outfile.write(sensor.data_string())


def actuator_state(state: int, data: [], actu_ex_pin: int, actu_re_pin: int):
    if state == EXT_FREE:
        Rpi.GPIO.output(actu_re_pin, Rpi.GPIO.LOW)
        Rpi.GPIO.output(actu_ex_pin, Rpi.GPIO.HIGH)
    elif state == RET_FREE:
        Rpi.GPIO.output(actu_ex_pin, Rpi.GPIO.LOW)
        Rpi.GPIO.output(actu_re_pin, Rpi.GPIO.HIGH)
    elif state == EXT_TIME:
        Rpi.GPIO.output(actu_re_pin, Rpi.GPIO.LOW)
        if time.time() < data[0]:
            Rpi.GPIO.output(actu_ex_pin, Rpi.GPIO.HIGH)
        else:
            Rpi.GPIO.output(actu_ex_pin, Rpi.GPIO.LOW)
    elif state == RET_TIME:
        Rpi.GPIO.output(actu_ex_pin, Rpi.GPIO.LOW)
        if time.time() < data[0]:
            Rpi.GPIO.output(actu_re_pin, Rpi.GPIO.HIGH)
        else:
            Rpi.GPIO.output(actu_re_pin, Rpi.GPIO.LOW)
    elif state == EXT_DIST:
        Rpi.GPIO.output(actu_re_pin, Rpi.GPIO.LOW)
        if r_potentiometer.value[0] < data[0] + data[1] or data is None:  # TODO check the left side as well as comparison
            Rpi.GPIO.output(actu_ex_pin, Rpi.GPIO.LOW)
        else:
            Rpi.GPIO.output(actu_ex_pin, Rpi.GPIO.LOW)
    elif state == RET_DIST:
        Rpi.GPIO.output(actu_ex_pin, Rpi.GPIO.LOW)
        if r_potentiometer.value[0] > data[0] + data[1]:  # TODO check the left side as well as comparison
            Rpi.GPIO.output(actu_ex_pin, Rpi.GPIO.LOW)
        else:
            Rpi.GPIO.output(actu_ex_pin, Rpi.GPIO.LOW)
    elif state == EXT_TARG:
        Rpi.GPIO.output(actu_ex_pin, Rpi.GPIO.LOW)
        if r_potentiometer.value[0] > data[0]:  # TODO check the left side as well as comparison
            Rpi.GPIO.output(actu_ex_pin, Rpi.GPIO.LOW)
        else:
            Rpi.GPIO.output(actu_ex_pin, Rpi.GPIO.LOW)
    elif state == RET_TARG:
        Rpi.GPIO.output(actu_ex_pin, Rpi.GPIO.LOW)
        if r_potentiometer.value[0] == data[0]:  # TODO check the left side as well as comparison
            Rpi.GPIO.output(actu_ex_pin, Rpi.GPIO.LOW)
        else:
            Rpi.GPIO.output(actu_ex_pin, Rpi.GPIO.LOW)
    elif state == HALT:
        Rpi.GPIO.output(actu_ex_pin, Rpi.GPIO.LOW)
        Rpi.GPIO.output(actu_ex_pin, Rpi.GPIO.LOW)


def update_actuators():
    if EBRAKE_EMAG_L_STATE == OFF:
        Rpi.GPIO.output(EBRAKE_EMAG_L, Rpi.GPIO.LOW)
    elif EBRAKE_EMAG_L_STATE == ON:
        Rpi.GPIO.output(EBRAKE_EMAG_L, Rpi.GPIO.HIGH)
    if EBRAKE_EMAG_R_STATE == OFF:
        Rpi.GPIO.output(EBRAKE_EMAG_R, Rpi.GPIO.LOW)
    elif EBRAKE_EMAG_R_STATE == ON:
        Rpi.GPIO.output(EBRAKE_EMAG_R, Rpi.GPIO.HIGH)

    actuator_state(EBRAKE_ACTU_L_STATE[0], EBRAKE_ACTU_L_STATE[1], EBRAKE_ACTU_L_FORW, EBRAKE_ACTU_L_BACK)
    actuator_state(EBRAKE_ACTU_R_STATE[0], EBRAKE_ACTU_R_STATE[1], EBRAKE_ACTU_R_FORW, EBRAKE_ACTU_R_BACK)
    actuator_state(MAINB_ACTU_L_STATE[0], MAINB_ACTU_L_STATE[1], MAINB_ACTU_L_FORW, MAINB_ACTU_L_BACK)
    actuator_state(MAINB_ACTU_R_STATE[0], MAINB_ACTU_R_STATE[1], MAINB_ACTU_R_FORW, MAINB_ACTU_R_BACK)


def send():
    pass


def recv():
    pass


if __name__ == "__main__":
    logfile = open("temp_log.txt")
    start = time.time()

    Rpi.GPIO.setup(EBRAKE_ACTU_L_BACK, Rpi.GPIO.OUT, initial=Rpi.GPIO.LOW)
    Rpi.GPIO.setup(EBRAKE_ACTU_L_FORW, Rpi.GPIO.OUT, initial=Rpi.GPIO.LOW)
    Rpi.GPIO.setup(EBRAKE_ACTU_R_BACK, Rpi.GPIO.OUT, initial=Rpi.GPIO.LOW)
    Rpi.GPIO.setup(EBRAKE_ACTU_R_FORW, Rpi.GPIO.OUT, initial=Rpi.GPIO.LOW)
    Rpi.GPIO.setup(EBRAKE_EMAG_L, Rpi.GPIO.OUT, initial=Rpi.GPIO.LOW)
    Rpi.GPIO.setup(EBRAKE_EMAG_L, Rpi.GPIO.OUT, initial=Rpi.GPIO.LOW)
    Rpi.GPIO.setup(MAINB_ACTU_L_FORW, Rpi.GPIO.OUT, initial=Rpi.GPIO.LOW)
    Rpi.GPIO.setup(MAINB_ACTU_L_BACK, Rpi.GPIO.OUT, initial=Rpi.GPIO.LOW)
    Rpi.GPIO.setup(MAINB_ACTU_R_FORW, Rpi.GPIO.OUT, initial=Rpi.GPIO.LOW)
    Rpi.GPIO.setup(MAINB_ACTU_R_BACK, Rpi.GPIO.OUT, initial=Rpi.GPIO.LOW)

    while True:
        update_sensors()
        log(logfile)
        send()
        recv()
        update_actuators()
