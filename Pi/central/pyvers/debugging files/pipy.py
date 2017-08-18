from socket import *
import threading

IP_ADDRESS = "192.168.1.11"  # CHANGE THIS TO COMPUTER'S IP ADDRESS!!!
PORT = 3002  # If you change this change in gui.py as well.

hostName = gethostbyname('0.0.0.0')
socketA = socket(AF_INET, SOCK_DGRAM)
socketA.bind((hostName, PORT))

def send_data2gui(data):
    """
    Updates GUI.
    :param data: List of data to be sent to GUI, numbers must be in string format in the following order...
        [ E-Brake Potentiometer, Height, Velocity, Acceleration, Distance Traveled ]
    """

    payload = " ".join(data)
    socketA.sendto(payload.encode(), (IP_ADDRESS, PORT))

def recieve_command():
    while True:
        (data, addr) = socketA.recvfrom(2048)
        command = data.decode()

        # TODO: ADD LOGIC FOR WHAT TODO WHEN RECIEVE COMMAND

if __name__ == "__main__":
    threading.Thread(target=recieve_command).start()  # Start this thread.


