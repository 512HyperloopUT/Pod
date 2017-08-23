import lib.hyper_io as io
import time

subsystems = []

class TestSubsystem():
    def __init__(self, podinput):
        self.podinput = podinput
        self.waittime = 20
        self.start = time.time()
        self.printed = False

        subsystems.append(self)

    def run(self):
        if time.time() - self.start > self.waittime:
            if podinput.cycles > 500 and not self.printed:
                self.printed = True
                print("hi there")

class PublishSubsystem():
    def __init(self, input):
        self.podinput = podinput
        subsystems.append(self)

    def run(self):
        #publish sensor data to UDP
        pass
