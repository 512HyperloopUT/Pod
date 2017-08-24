#!/usr/bin/env/ python3
# Using python 3
# Testing TM4C-Pi connection

from hyper import comms

if __name__ == "__main__":
    comm_port = comms.CommPort()
    while True:
        print('ra = read, rd = read, w = write, q = quit')
        choice = input("choice: ")
        if choice == 'ra':
            readID = int(input("readID: "))
            val = comm_port.readAnalog(readID)
            vout = (val * 5.0) / 1024
            vin = vout / (7500 / (30000 + 7500))
            vin = vin * .175
            print('read: ', vin)
        elif choice == 'rd':
            readID = int(input("readID: "))
            print("read: ", comm_port.readDigital(readID))
        elif choice == 'w':
            writeID = int(input("writeID: "))
            direction = comms.WriteDir(int(input("dir: ")))
            comm_port.writeDigitalActuator(writeID, direction)
        elif choice == 'q':
            break
        else:
            print('invalid option')
