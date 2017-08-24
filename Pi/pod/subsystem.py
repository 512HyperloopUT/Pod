from hyper import io

class TestSubsystem:
    def __init__(self, input_data):
        self.input_data = input_data
        self.printed = False

    def run(self):
        if self.input_data.duration > 2 and not self.printed:
            self.printed = True
            print("hi there")

class EMagSubsystem:
    def __init__(self, input_data):
        self.input_data = input_data
        self.emag = io.EMag(-1, input_data.comm_port)

    def run(self):
        self.emag.set(self.input_data.emag_activated)

class PublishSubsystem:
    def __init__(self, input_data):
        self.input_data = input_data

    def run(self):
        # publish sensor data to UDP
        pass
