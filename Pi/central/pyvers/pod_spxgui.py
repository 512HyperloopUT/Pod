"""
<code>master_packet</code> is a packet that is sent to the computer with a GUI. It is an abstract class.

<code>master</code> is the computer that handles the GUI. It is an abstract class.
"""


class MasterPacket:
    def __init__(self):
        pass


class Master:
    def send(self, packet: MasterPacket):
        pass

    def receive(self):
        pass
