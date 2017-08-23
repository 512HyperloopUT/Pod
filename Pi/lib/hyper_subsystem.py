import lib.hyper_io as io
import time

class Subsystem:
    def __init__(self, input):
        pass

    def run(self):
        pass

class TestSubsystem(Subsystem):
    def __init__(self, input):
        super().__init__(input)
        self.waittime = 20
        self.start = time.time()
        self.printed = False

    def run(self):
        if time.time() - self.start > self.waittime:
            if input.cycles > 500 and not self.printed:
                self.printed = True
                print("hi there")
