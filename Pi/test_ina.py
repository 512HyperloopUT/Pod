#!/usr/bin/env python
from ina219 import INA219
from ina219 import DeviceRangeError
import logging

def read():
    ina = INA219(0.1)
    ina.configure()

    while True:
        print("curr: ", ina.current())

if __name__ == "__main__":
    read()
