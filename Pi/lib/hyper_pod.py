import lib.hyper_io as io
import lib.hyper_subsystem as subsystems

class Pod:
    def __init__(self):
        self.podinput = io.Input()
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
            self.input.update()
            for subsystem in self.subsystems:
                subsystem.run()
            if self.input.cycles > 1000:
                running = False
