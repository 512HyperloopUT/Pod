#!/usr/bin/env python3
# Using python 3

# set up i2c on the pi
# first, un-blacklist i2c
# next, add the i2c modules to the thing
# afterthat, install i2c-tools and reboot
# finally, run i2cdetect

# install smbus2

import time

import smbus2

import pod_periph


class IMUSensor(pod_periph.I2CSensor):
    def update_sensor(self, commt: pod_periph.Client):
        super(commt)
        # TODO Analyze data


BRAKE_INTERVAL = 10 # Change to change num. of seconds before braking
sensor_ports = [
    IMUSensor("imu", 1, 0x28),
    pod_periph.AnalogSensor("prox0", 0),
    pod_periph.AnalogSensor("prox1", 1),
    pod_periph.AnalogSensor("prox2", 2),
    pod_periph.AnalogSensor("prox3", 3)
]
cli = pod_periph.Client()


def update_sensors():
    for sensor in sensor_ports:
        sensor.update_sensor(cli)


def log(outfile):
    for sensor in sensor_ports:
        outfile.write(sensor.data_string())


if __name__ == "__main__":
    logfile = open("temp_log.txt")
    start = time.time()
    while time.time() - start < BRAKE_INTERVAL:
        update_sensors()
        log(logfile)
    # TODO Add in the braking
