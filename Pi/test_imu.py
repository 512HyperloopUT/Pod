#!/usr/bin/env python3
# Using python 3
# Testing the imu

from hyper import io

ADDR = 0x28

if __name__ == "__main__":
    imu = io.IMUSensor("imu", ADDR)
    while True:
        imu.update()
        print(imu.data_string())
