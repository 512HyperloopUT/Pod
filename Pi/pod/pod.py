from hyper import io
from pod import subsystem


class Pod:
    def __init__(self):
        self.podinput = io.Input()
        self.subsystems = []
        self.subsystems.append(subsystem.TestSubsystem(self.podinput))
        self.subsystems.append(subsystem.PublishSubsystem(self.podinput))

    def loop(self):
        running = True
        while running:
            self.podinput.update()
            for sub in self.subsystems:
                sub.run()
            if self.podinput.duration > 5:
                running = False

    def start(self):
        # test comms
        # calibrate
        # cycle electromags
        # cycle actuators
        self.loop()
