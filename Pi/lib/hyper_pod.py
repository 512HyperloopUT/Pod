import lib.hyper_io as io
import lib.hyper_comms as comms
import lib.hyper_subsystem as subsystems

class Pod:
    def __init__(self):
        self.podinput = io.Input()
        self.subsystems = []
        self.subsystems.append(subsystems.TestSubsystem(self.podinput))

    def loop(self):
        running = True
        while running:
            self.podinput.update()
            for subsystem in self.subsystems:
                subsystem.run()
            if self.podinput.duration > 5:
                running = False

    def start(self):
        #test comms
        #calibrate
        #cycle electromags
        #cycle actuators
        self.loop()
