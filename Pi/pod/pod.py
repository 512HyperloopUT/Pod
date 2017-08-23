from hyper import io
from pod import subsystem


class Pod:
    def __init__(self):
        self.input_data = io.Input()
        self.subsystems = []
        self.subsystems.append(subsystem.TestSubsystem(self.input_data))
        self.subsystems.append(subsystem.PublishSubsystem(self.input_data))

    def loop(self):
        running = True
        while running:
            self.input_data.update()
            for sub in self.subsystems:
                sub.run()
            if self.input_data.duration > 5:
                running = False

    def start(self):
        if not self.input_data.isReady():
            return
        # cycle electromags
        # cycle actuators
        self.loop()
