#!/usr/bin/env python3
# Using python 3

import time
import lib.hyper_periph as periph

ADDR = 0x28

if __name__ == "__main__":
    imu = periph.IMUSensor("imu", ADDR)
    while True:
        print("Updating sensor.")
        imu.update_sensor()
        print(imu.data_string())
