import lib.hyper_io as io
import lib.hyper_subsystem as subsystems

class Pod:
    def __init__(self):
        self.podinput = io.Input()
        self.subsystems = []
        self.subsystems.append(subsystems.TestSubsystem(podinput))

    def start(self):
        #test comms
        #calibrate
        #cycle electromags
        #cycle actuators
        loop()

    def loop(self):
        running = True
        while running:
            self.podinput.update()
            for subsystem in subsystems:
                subsystem.run()
            if self.podinput.cycles > 1000:
                running = False
