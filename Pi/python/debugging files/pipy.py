from socket import *
import threading

import random
import time

IP_ADDRESS = "192.168.1.11"  # CHANGE THIS TO COMPUTER'S IP ADDRESS!!!
PORT = 3002  # If you change this change in gui.py as well.

hostName = gethostbyname('0.0.0.0')
socketA = socket(AF_INET, SOCK_DGRAM)
socketA.bind((hostName, PORT))

def send_data2gui(data):
    """
    :param data: List of data to be sent to GUI, numbers must be in string format
    """

    payload = " ".join(data)
    socketA.sendto(payload.encode(), (IP_ADDRESS, PORT))

def recieve_command():
    while True:
        (data, addr) = socketA.recvfrom(2048)
        print (data)

if __name__ == "__main__":
    threading.Thread(target=recieve_command).start()
    stuff=[]
    while True:
        for _ in range(5):
            stuff.append(str(random.random()))
        send_data2gui(stuff)
        time.sleep(1)


