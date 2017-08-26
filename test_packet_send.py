from socket import *
from Pi.hyper.io import *
import time

send_socket = socket(AF_PACKET, SOCK_RAW)

IP_ADDRESS = ''
PORT = 3000

while True:
    input_pi = Input()
    send_array = [input_pi.battery_current]
    send_string = ' '.join(send_array)

    send_socket.sendto(send_string.encode(), (IP_ADDRESS, PORT))

    time.sleep(1)
