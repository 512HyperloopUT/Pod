from socket import *

send_socket = socket(AF_PACKET, SOCK_RAW)

IP_ADDRESS = ''
PORT = 3000

send_socket.sendto('hi'.encode(), (IP_ADDRESS, PORT))
