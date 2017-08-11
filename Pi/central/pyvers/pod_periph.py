# Using python 3

"""
<code>sensor</code> is used read an input from a local hardware device.

<code>actuator</code> is used write an output from a local hardware device. It is an abstract class.

Both of these classes use a static factory method.

<code>client</code> refers to and abstract connection to the pod hardware, combining GPIO, UART and the communication with the TM4C (which uses GPIO).
"""


class Sensor:
    def __init__(self, name: str):
        self.name = name

    def update_sensor(self, commt: client):
        print("Undefined behavior")


def make_sensor(name: str):
    # TODO for different sensors
    pass


class Actuator:
    def __init__(self, name: str):
        self.name = name

    def update_actuator(self, commt: client, val: bool):
        print("Undefined behavior")


class client:
    def __init__(self):
        pass

    def write(self, id: int, value: bool):
        pass

    def read(self, id):
        pass
