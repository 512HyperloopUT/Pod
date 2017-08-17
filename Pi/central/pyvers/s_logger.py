#!/usr/bin/env python3
# Using python 3

# set up i2c on the pi
# first, un-blacklist i2c
# next, add the i2c modules to the loaded kernel modules
# afterthat, install i2c-tools and reboot
# finally, run i2cdetect

# install smbus2

import time
import Rpi.GPIO

import pod_periph


class IMUSensor(pod_periph.I2CSensor):
    def update_sensor(self, commt: pod_periph.Client):
        super(commt)
        # TODO Analyze data


BRAKE_INTERVAL = 10  # Change to change num. of seconds before braking
potentiometer = pod_periph.AnalogSensor("emergencypotent", 2)
sensor_ports = [
    IMUSensor("imu", 1, 0x28),
    pod_periph.AnalogSensor("prox0", 0),
    pod_periph.AnalogSensor("prox1", 1),
    potentiometer
]
cli = pod_periph.Client()

RELAY_EX_PIN_NUM = 10
RELAY_RE_PIN_NUM = 11
EXTEND_LIMIT = 500
RETRACT_LIMIT = 500


def update_sensors():
    for sensor in sensor_ports:
        sensor.update_sensor(cli)


def log(outfile):
    for sensor in sensor_ports:
        outfile.write(sensor.data_string())


if __name__ == "__main__":
    logfile = open("temp_log.txt")
    start = time.time()
    Rpi.GPIO.setup(RELAY_EX_PIN_NUM, Rpi.GPIO.OUT, initial=Rpi.GPIO.LOW)
    Rpi.GPIO.setup(RELAY_RE_PIN_NUM, Rpi.GPIO.OUT, initial=Rpi.GPIO.LOW)
    while time.time() - start < BRAKE_INTERVAL:
        update_sensors()
        log(logfile)

    # TODO Add in the braking
    # TODO CHECK IF THIS WORKS
    Rpi.GPIO.output(RELAY_EX_PIN_NUM, Rpi.GPIO.HIGH)

    potentiometer.update_sensor()
    while potentiometer.value > EXTEND_LIMIT:
        potentiometer.update_sensor(cli)

    Rpi.GPIO.output(RELAY_EX_PIN_NUM, Rpi.GPIO.LOW)
    Rpi.GPIO.output(RELAY_RE_PIN_NUM, Rpi.GPIO.HIGH)

    potentiometer.update_sensor()
    while potentiometer.value < RETRACT_LIMIT:
        potentiometer.update_sensor(cli)

    Rpi.GPIO.output(RELAY_RE_PIN_NUM, Rpi.GPIO.LOW)
