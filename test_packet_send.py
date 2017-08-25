from socket import *

send_socket = socket(AF_INET, SOCK_RAW)

IP_ADDRESS = ''
PORT = 3000

send_socket.sendto('hi'.encode(), (IP_ADDRESS, PORT))
