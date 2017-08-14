# Using python 3
import random
import struct
from datetime import time
from socket import socket, AF_INET, SOCK_DGRAM, gethostbyname

isDebug = True


class Port:
    def __init__(self, server_ip):
        self.SERVER_IP = server_ip
        self.PORT_NUMBER = 3000
        self.SEND_PORT = ('localhost', 8889)
        self.SIZE = 1024

        self.socket = socket(AF_INET, SOCK_DGRAM)
        self.socket.setblocking(False)
        self.hostname = gethostbyname('0.0.0.0')
        self.socket.bind((self.hostname, self.PORT_NUMBER))

        self.prev = None
        self.packer = struct.Struct('! B B i i i i i i i I')

    def _pack(self, data: []):
        for i in range(10 - len(data)):
            data.append(0)
        return self.packer.pack(*data)

    def send(self, data: []):
        packed = self._pack(data)
        if not self.prev == packed:
            try:
                self.socket.sendto(packed, (self.SERVER_IP, self.PORT_NUMBER))
                self.prev = packed
            except Exception as e:
                # print(e)
                pass

    def receive(self):
        try:
            (load, addr) = self.socket.recvfrom(self.SIZE)
            return load
        except socket.timeout:
            return "None"


if __name__ == "__main__":
    pass
