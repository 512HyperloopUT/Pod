import lib.hyper_periph as periph
import lib.hyper_subsystem as subsystem

class Pod:
    def __init__(self):
        self.sensors = [periph.AnalogSensor("analogtest0", 0)]
        self.subsystems = [subsystem.TestSubsystem(sensors)]

    def loop():
        running = True
        while running:
            pass
