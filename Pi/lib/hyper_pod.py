import lib.hyper_io as io
import lib.hyper_subsystem as subsystems

class Pod:
    def __init__(self):
        self.input = io.Input()
        self.subsystems.append(subsystems.TestSubsystem(input))

    def start():
        #test comms
        #calibrate
        #cycle electromags
        #cycle actuators
        loop()

    def loop():
        running = True
        while running:
            self.input.update()
            for subsystem in self.subsystems:
                subsystem.run()
            if self.input.cycles > 1000:
                running = False
