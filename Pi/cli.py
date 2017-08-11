import random
import time
import struct
import RPi.GPIO as GPIO
from socket import socket, AF_INET, SOCK_DGRAM, gethostbyname
import threading

isDebug = True

GPIO.setMode(GPIO.BOARD)
GPIO.setup(12, GPIO.OUT)
GPIO.output(12, GPIO.LOW)

SERVER_IP = '192.168.0.' + raw_input("Server IP: ")
PORT_NUMBER = 3000
SEND_PORT = ('localhost', 8889)
SIZE = 1024

mySocket = socket(AF_INET, SOCK_DGRAM)
# mySocket.connect((SERVER_IP, PORT_NUMBER))
sendSocket = socket(AF_INET, SOCK_DGRAM)
hostName = gethostbyname('0.0.0.0')
mySocket.bind((hostName, PORT_NUMBER))
inputData = []


def fillData():
    """
    :return: A string with the encoded data received from pod.
    """
    global inputData
    inputData = []
    if isDebug:
        for i in range(8):
            inputData.append(int(random.random() * 10))
        time.sleep(.5)
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
                    print "EMERGENCY STOP"
                else:
                    sendSocket.sendto('-1', SEND_PORT)
            else:
                if isDebug:
                    print seconds[1]
                else:
                    if seconds[1] == 'reset':
                        GPIO.output(12, GPIO.HIGH)
                        time.sleep(.1)
                        GPIO.output(12, GPIO.LOW)
                    else:
                        sendSocket.sendto(seconds[1], SEND_PORT)
        else:
            inputData = seconds

if __name__ == "__main__":
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
