from hyper import io
from pod import subsystem


class Pod:
    def __init__(self):
        self.input_data = io.Input()
        self.subsystems = []
        self.subsystems.append(subsystem.LateralSubsystem(self.input_data))
        self.subsystems.append(subsystem.LevitationSubsystem(self.input_data))
        self.subsystems.append(subsystem.BrakeSubsystem(self.input_data))
        self.subsystems.append(subsystem.EBrakeSubsystem(self.input_data))
        self.subsystems.append(subsystem.BatterySubsystem(self.input_data))
        self.subsystems.append(subsystem.TestSubsystem(self.input_data))
        #self.subsystems.append(subsystem.PublishSubsystem(self.input_data))
        self.subsystems.append(subsystem.LogSubsystem(self.input_data))

    def loop(self):
        running = True
        while running:
            self.input_data.update()
            for sub in self.subsystems:
                sub.run()
            if self.input_data.duration > 5000:
                running = False

    def start(self):
        print("running tests...")
        if not self.input_data.isReady():
            print("- comms test FAILED")
            return
        print("- comms test passed")
        # cycle electromags
        # cycle actuators
        print("starting pod...")
        self.loop()
        print("safely exited pod")
