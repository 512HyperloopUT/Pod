import lib.hyper_io as io
import time

subsystems = []

class TestSubsystem():
    def __init__(self, podinput):
        self.podinput = podinput
        self.printed = False

        subsystems.append(self)

    def run(self):
        if podinput.duration > 2 and not self.printed:
            self.printed = True
            print("hi there")

class PublishSubsystem():
    def __init(self, input):
        self.podinput = podinput
        subsystems.append(self)

    def run(self):
        #publish sensor data to UDP
        pass