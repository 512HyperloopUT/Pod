import lib.hyper_io as io
import time
from socket import socket, AF_INET, SOCK_DGRAM


class TestSubsystem():
    def __init__(self, podinput):
        self.podinput = podinput
        self.printed = False

    def run(self):
        if self.podinput.duration > 2 and not self.printed:
            self.printed = True
            print("hi there")


class PublishSubsystem():
    def __init__(self, podinput):
        self.podinput = podinput
        self.gui_socket = socket(AF_INET, SOCK_DGRAM)
        self.last_send = podinput.duration()

        self.SERVER_IP = '192.168.0.1'
        self.PORT = 3000

    def run(self):
        if self.podinput.duration() != self.last_send:
            self.last_send = self.podinput.duration()
            self.gui_socket.sendto(self.podinput.get_packed_data(), (self.SERVER_IP, self.PORT))
