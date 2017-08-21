#!/usr/bin/env python3
# Using python 3

import pod_periph

BUS = 1
ADDR = 0x28

if __name__ == "__main__":
    cli = pod_periph.Client()
    imu = pod_periph.IMUSensor("imu", ADDR)
    while True:
        print("Updating sensor. Values:")
        imu.update_sensor(cli)
        print("\tdistance, magnitude: " + imu.value[0])
        print("\tvelocity: " + imu.value[1])
        print("\tcurrent orientation: " + imu.value[2])
        print("\tcurrent acceleration: " + imu.value[3])
        print("\tcurrent time: " + imu.value[4])