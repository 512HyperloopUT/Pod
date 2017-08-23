import lib.hyper_periph as periph
import time

class Subsystem:
    def __init__(self, sensors):
        pass

    def run():
        pass

class EBrake(Subsystem):
    def __init__(self, sensors):
        super().__init__(sensors)
        self.left_emag = periph.Actuator("left_emag", 13)
        self.right_emag = periph.Actuator("right_emag", 14)
        self.waittime = 20
        self.start = time.time()

    def run():
        if time.time() - self.start > self.waittime:
            if sensors[0].value > 500:
                self.left_emag.set(2)
                self.right_emag.set(2)
