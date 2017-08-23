#!/usr/bin/env python3
# Using python 3

import time
import lib.hyper_periph as periph

ADDR = 0x28

if __name__ == "__main__":
    imu = periph.IMUSensor("imu", ADDR)
    while True:
        print("Updating sensor. Values:")
        imu.update_sensor()
        print("\tdistance:", imu.value[0])
        print("\tvelocity:", imu.value[1])
        print("\tcurrent orientation:", imu.value[2])
        print("\tcurrent acceleration:", imu.value[3])
        print("\tcurrent time:", imu.value[4])
        time.sleep(0.2)
