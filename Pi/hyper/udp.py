from socket import socket, gethostbyname, AF_INET, SOCK_DGRAM


class UDPClient:
    def __init__(self):
        self.PORT = 3000
        self.rec_socket = socket(AF_INET, SOCK_DGRAM)

        self.rec_socket.setblocking(0)
        hostname = gethostbyname('0.0.0.0')
        self.rec_socket.bind((hostname, 3000))

        self.ebrake = False
        self.timer = -1
        self.user_command = None

    def update(self):
        try:
            rec_string = self.rec_socket.recv()
            command_components = rec_string.decode().split()
            if command_components[0] == 's' * 35:  # Should always be true, else data is corrupted or something.
                if command_components[1] == 'stop':
                    self.ebrake = True
                elif command_components[1] == 'time':
                    self.timer = int(command_components[2])
                elif command_components[1] == 'cmd':
                    self.user_command = command_components[2]
        except: # Didn't get anything.
            pass

