#!/usr/bin/env python3
# Using python 3

import pod_periph
import mag from quat

BUS = 1
ADDR = 0x28

if __name__ == "__main__":
    imu = pod_periph.IMUSensor("imu", ADDR)
    while True:
        print("Updating sensor. Values:")
        imu.update_sensor()
        print("\tdistance, magnitude: " + imu.values[0])
        print("\tvelocity: " + imu.values[1])
        print("\tcurrent orientation: " + imu.values[2])
        print("\tcurrent acceleration: " + imu.values[3])
        print("\tcurrent time: " + imu.values[4])