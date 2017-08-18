from socket import *
import threading
from tkinter import *
from tkinter.ttk import *
import time

measurementList = ("E-Brake Potentiometer", "Height", "Velocity", "Acceleration", "Distance Traveled")
defaultValues = "0" * len(measurementList)

IP_INFO = ('', '')  # This will update after first batch of data is recieved.
PORT = 3002  # If you change this change in gui.py as well.

hostName = gethostbyname('0.0.0.0')
socketA = socket(AF_INET, SOCK_DGRAM)
socketA.bind((hostName, PORT))

class ServerFrame(Frame):
    """
    Singleton GUI frame
    """
    def __init__(self, parent):
        Tk.frame.__init__(self, parent)
        s = Style()
        s.theme_use('default')
        self.parent = parent
        self.ip_label = Label(parent, text="Local IP Address: " + get_ip(), background='white')
        self.client_tree = Treeview(parent, columns='tree')
        self.command_box = Entry(parent)
        self.command_button = Button(parent, text="Send", command=send_command)

        self.parent.title("Debug Panel")

        self.ip_label.pack()
        self.client_tree.heading('#0', text="Element")
        self.client_tree.heading('#1', text="Value")
        self.client_tree.column('#0')
        self.client_tree.column('#1')
        self.client_tree.pack()
        self.command_box.pack()
        self.command_button.pack()

        self.init_data()

    def init_data(self):
        """ Initializes treeview with default numbers"""
        for i in range(len(measurementList)):
            self.client_tree.insert('', 'end', iid=i + 1, text=measurementList[i], values=defaultValues[i])


def get_ip():
    s = socket(AF_INET, SOCK_DGRAM)
    s.connect(('192.0.0.8', 1027))
    return s.getsockname()[0]

def send_command():
    command = main_frame.command_box.get()
    try:
        socketA.sendto(command.encode(), IP_INFO)
    except TypeError:
        print("You have to have the Pi initiate the connection first")
    main_frame.command_box.delete(0, 'end')

def update_data():
    global IP_INFO
    time.sleep(.5)
    while True:
        (data, addr) = socketA.recvfrom(2048)
        IP_INFO = addr
        data_list = data.split()
        for i in range(len(measurementList)):
            main_frame.client_tree.set(i + 1, column="#1", value=data_list[i])

if __name__ == "__main__":
    threading.Thread(target=update_data).start()
    root = Tk()
    ServerFrame.root = root
    main_frame = ServerFrame(root)
    main_frame.parent.mainloop()

