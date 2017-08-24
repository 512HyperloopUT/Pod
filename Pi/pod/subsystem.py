import logging
import csv
from socket import socket, AF_INET, SOCK_DGRAM

from hyper import io


# HARDWARE SUBSYSTEMS


class LevitationSubsystem:
    def __init__(self, input_data):
        self.input_data = input_data
        self.lf_lev = io.Actuator(0, input_data.comm_port)
        self.rf_lev = io.Actuator(1, input_data.comm_port)
        self.lb_lev = io.Actuator(2, input_data.comm_port)
        self.rb_lev = io.Actuator(3, input_data.comm_port)

    def run(self):
        pass


class LateralSubsystem:
    def __init__(self, input_data):
        self.input_data = input_data
        self.lf_lat = io.Actuator(4, input_data.comm_port)
        self.rf_lat = io.Actuator(5, input_data.comm_port)
        self.lb_lat = io.Actuator(6, input_data.comm_port)
        self.rb_lat = io.Actuator(7, input_data.comm_port)

    def run(self):
        pass


class BrakeSubsystem:
    def __init__(self, input_data):
        self.input_data = input_data
        self.l_br = io.Actuator(10, input_data.comm_port)
        self.r_br = io.Actuator(11, input_data.comm_port)

    def run(self):
        pass


class EBrakeSubsystem:
    def __init__(self, input_data):
        self.input_data = input_data
        self.l_ebr = io.Actuator(8, input_data.comm_port)
        self.r_ebr = io.Actuator(9, input_data.comm_port)
        self.ebr_emag = io.DigitalOutput(13, input_data.comm_port)

    def run(self):
        pass


class BatterySubsystem:
    def __init__(self, input_data):
        self.input_data = input_data
        self.battery_switch = io.DigitalOutput(12, input_data.comm_port)

    def run(self):
        pass


# SOFTWARE SUBSYSTEMS


class TestSubsystem:
    def __init__(self, input_data):
        self.input_data = input_data
        self.printed = False

    def run(self):
        if self.input_data.duration > 2000 and not self.printed:
            self.printed = True
            print("hi there")


class PublishSubsystem():
    def __init__(self, podinput):
        self.podinput = podinput
        self.gui_socket = socket(AF_INET, SOCK_DGRAM)
        self.last_send = self.podinput.duration

        self.SERVER_IP = '192.168.0.1'
        self.PORT = 3000

    def run(self):
        if self.podinput.duration - self.last_send >= 500:
            self.last_send = self.podinput.duration
            self.gui_socket.sendto(
                self.podinput.get_packed_data(), (self.SERVER_IP, self.PORT))


class LogSubsystem:
    def __init__(self, input_data):
        self.input_data = input_data
        with open('log.csv', 'w', newline='') as csvfile:
            csv_writer = csv.writer(
                csvfile, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
            self.csv_writer = csv_writer
            csv_writer.writerow(['status', 'x-acceleration', 'y-acceleration', 'z-acceleration', 'w-orientation',
                                 'x-orientation', 'y-orientation', 'z-orientation', 'voltage', 'current'])

    def run(self):
        i = self.input_data
        data_row = [i.status, i.accelerationX, i.accelerationY, i.accelerationZ,
                    i.OriW, i.OriX, i.OriY, i.OriZ, i.voltage, i.amperage_highcurrent]
        self.csv_writer.writerow(data_row)
