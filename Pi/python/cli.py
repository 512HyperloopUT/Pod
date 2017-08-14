#!/usr/bin/env python
# Using Python 3

import random
import time
import struct
import RPi.GPIO as GPIO
from socket import socket, AF_INET, SOCK_DGRAM, gethostbyname
import threading
import main

isDebug = True

SERVER_IP = raw_input("Server IP: ")
PORT_NUMBER = 3000
SEND_PORT = ('localhost', 8889)
SIZE = 1024

mySocket = socket(AF_INET, SOCK_DGRAM)
sendSocket = socket(AF_INET, SOCK_DGRAM)
hostName = gethostbyname('0.0.0.0')
mySocket.bind((hostName, PORT_NUMBER))
inputData = []


def process(sensorReads):
    """

    :param sensorReads: A list containing the values the sensors return.
    :return: A list of numbers in the format dictated by the SpaceX manual. (teamID, status, position, velocity, ...)
    """

    return []  # TODO: PETER, write this code

def fillData():
    """
    :return: A string with the encoded data received from pod.
    """
    global inputData
    inputData = []
    if isDebug:
        for i in range(8):
            inputData.append(int(random.random() * 10))
        time.sleep(1)
    else:
        readIDs = (1, 2, 3)  # TODO: PETER, replace these values with the actual sensor IDs in the order you want.
        sensorReads = [] # List of values received from sensors
        for ID in readIDs:
            setRead(ID)
            # Maybe wait a while?? -> time.sleep(.1)
            sensorReads.append(readUART())
        inputData = process(sensorReads)
    for i in range(10 - len(inputData)):
        inputData.append(0)
    packer = struct.Struct('! B B i i i i i i i I')
    packeddata = packer.pack(*inputData)
    return packeddata


def checkButton():
    """
    Continously checks for...
        - Signal that emergency brake should activate.
        - A time update.
        - New output data from localhost
    """
    global inputData
    while True:
        (load, addr) = mySocket.recvfrom(SIZE)
        seconds = load.split()
        if load.startswith('s'*35):
            if len(seconds) == 1:
                if isDebug:
                    print("EMERGENCY STOP")
                else:
                    # TODO: handle stop
                    # sendSocket.sendto('-1', SEND_PORT)
                    pass
            else:
                if isDebug:
                    print(seconds[1])
                else:
                    if seconds[1] == 'reset':  # If a reset is triggered on the GUI
                        setRead(31) # TODO: PETER, Check if this is correct
                    else: # If a time update is given on the GUI, number sent is contained in seconds[1]
                        # TODO: handle time set
                        #sendSocket.sendto(seconds[1], SEND_PORT)
                        pass
        else:
            inputData = seconds


if __name__ == "__main__":
    GPIOsetup()
    threading.Thread(target=checkButton).start()
    prevData = [0] * 10
    while True:
        data = fillData()
        if not data == prevData:
            try:
                mySocket.sendto(data, (SERVER_IP, PORT_NUMBER))
                prevData = data
            except:
                # Not a big deal just try again...
                time.sleep(.5)
