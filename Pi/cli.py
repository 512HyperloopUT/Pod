import random
import time, struct
from socket import socket, AF_INET, SOCK_DGRAM, gethostbyname
import threading

SERVER_IP = raw_input("Server IP: ")
PORT_NUMBER = 3000
SIZE = 1024

mySocket = socket(AF_INET, SOCK_DGRAM)
#mySocket.connect((SERVER_IP, PORT_NUMBER))
hostName = gethostbyname('0.0.0.0')
mySocket.bind((hostName, PORT_NUMBER))

def fillData():
    """
    :return: A string with the encoded data received from pod.
    """
    n = 10
    inputdata = ()
    for i in range(n):
        inputdata += (int(random.random()*10),)
    for i in range(10 - len(inputdata)):
        inputdata += (0,)
    packer = struct.Struct('! B B i i i i i i i I')
    packeddata = packer.pack(*inputdata)
    return packeddata

def checkButton():
    """
    Continously checks for signal that emergency stop has activated.
    """
    while True:
        (load, addr) = mySocket.recvfrom(35)
        if load == 's'*35:
            print "EMERGENCY STOP"

threading.Thread(target=checkButton).start()
while True:
    data = fillData()
    try:
        mySocket.sendto(data, (SERVER_IP, PORT_NUMBER))
    except:
        pass
    time.sleep(.2)
