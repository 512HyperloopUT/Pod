import logging
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


class LogSubsystem:
    def __init__(self, input_data):
        self.input_data = input_data

        self.logger = logging.getLogger(str(self.input_data.team_id))
        self.logger.setLevel(logging.INFO)

        # create a file handler
        self.handler = logging.FileHandler("status.log")
        self.handler.setLevel(logging.INFO)

        # create a logging format
        self.formatter = logging.Formatter(
            "%(asctime)s - %(name)s - %(levelname)s - %(message)s")
        self.handler.setFormatter(self.formatter)

        # add the handlers to the logger
        self.logger.addHandler(self.handler)

    def run(self):
        self.logger.info('\nStatus:' + str(self.input_data.status) +
                         '\nAcceleration:)' + str(self.input_data.acceleration))
