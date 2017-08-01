import random
import time, struct
from socket import socket, AF_INET, SOCK_DGRAM


SERVER_IP = raw_input("Server IP: ")
PORT_NUMBER = 3000
SIZE = 1024

mySocket = socket(AF_INET, SOCK_DGRAM)
mySocket.connect((SERVER_IP, PORT_NUMBER))

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

while True:
    data = fillData()
    try:
        mySocket.sendall(data)
    except:
        pass
    time.sleep(.2)
