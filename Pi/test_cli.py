#!/usr/bin/env/ python3
# Using python 3
# Testing TM4C-Pi connection

from hyper import comms

if __name__ == "__main__":
    comm_port = comms.CommPort()
    while True:
        print('r = read, w = write, q = quit')
        choice = input("choice: ")
        if choice == 'r':
            readID = int(input("readID: "))
            print('read: ', comm_port.read(readID))
        elif choice == 'w':
            writeID = int(input("writeID: "))
            direction = comms.WriteDir(int(input("dir: ")))
            comm_port.write(writeID, direction)
        elif choice == 'q':
            break
        else:
            print('invalid option')
