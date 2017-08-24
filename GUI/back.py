#!/usr/bin/env python
# Using Python 2

"""
This GUI code can send the following commands to the Pod.
All commands are prefaced by 35 's' characters... for reasons

- E-brake
    'stop'
[- Reset Board
    'reset'] -> Deprecated
- Set Time
    'time' 'n'  n being time in seconds
- Send Command
    'cmd' 's'   s being a command to be parsed by someone else
"""

from Tkinter import *
import Tkinter
import ttk
import threading
import struct
from socket import socket, gethostbyname, AF_INET, SOCK_DGRAM

measurementList = "Team_ID Status Acceleration Position Velocity battery_voltage battery_current battery_temperature " \
                  "pod_temperature stripe_count".split()
defaultValues = [0] * len(measurementList)

PORT_NUMBER = 3000
hostName = gethostbyname('0.0.0.0')
mySocket = socket(AF_INET, SOCK_DGRAM)
mySocket.bind((hostName, PORT_NUMBER))
SERVER_IP = ''


class Frame(Tkinter.Frame):
    def __init__(self, parent):
        """ Sets up instance variables"""
        Tkinter.Frame.__init__(self, parent)
        self.parent = parent
        self.tree = ttk.Treeview(self.parent, columns='data')
        self.button = Button(self.parent, text="Emergency Stop", command=cmdStop)
        self.entry = Entry(self.parent)
        self.time_button = Button(self.parent, text="Send Time", command=cmdTime)
        self.cmd_entry = Entry(self.parent)
        self.cmd_button = Button(self.parent, text="Send Command", command=cmdCommand)
        self.reset_button = Button(self.parent, text="Reset TI Board", command=cmdReset)
        self.text_box = Text(self.parent)

        self.current_data = defaultValues

        self.initialize_user_interface()
        self.init_data()

    def initialize_user_interface(self):
        """ Sets up user interface"""
        self.parent.title("512 Hyperloop Display Panel: No client connected")
        self.parent.grid_rowconfigure(0, weight=1)
        self.parent.grid_columnconfigure(0, weight=1)
        self.parent.config(background="white")

        self.tree.heading('#0', text='Feature')
        self.tree.heading('#1', text='Data')
        self.tree.column("#0", stretch=Tkinter.YES)
        self.tree.column("#1", stretch=Tkinter.YES, anchor='e')
        self.tree.tag_configure('orange', background='orange')
        self.tree.tag_configure('red', background='red')
        self.tree.grid(row=0, columnspan=1, sticky='new')
        self.button.grid(row=1, columnspan=1, sticky='n')
        self.entry.grid(row=2, columnspan=1, sticky='nw')
        self.time_button.grid(row=2, columnspan=2, sticky='ne')
        #self.reset_button.grid(row=3, columnspan=3, sticky='n')
        self.cmd_entry.grid(row=4, columnspan=1, sticky='nw')
        self.cmd_button.grid(row=4, columnspan=2, sticky='ne')
        self.text_box.grid(row=5, columnspan=1, sticky='n')
        self.text_box.insert(END, "Warnings: \n")

    def init_data(self):
        """ Initializes treeview with default numbers"""
        for i in range(len(measurementList)):
            self.tree.insert('', 'end', iid=i + 1, text=measurementList[i], values=defaultValues[i])

    def complain(self, string):
        """ Prints out problems to user"""
        top = Toplevel()
        top.title("Error")
        msg = Message(top, text=string, width=1000)
        msg.pack(side="top", padx=10, pady=10)
        button = Button(top, text="Dismiss", command=top.destroy)
        button.pack()

    def check_warnings(self):
        # TODO: Orientation/Flyheight and Voltmeter
        for i in range(10):
            self.tree.item(i + 1, tags="normal")
        if self.current_data[1] == 3:
            if self.current_data[4] >= 90:
                self.text_box.insert(END, "Velocity > 90\n")
                self.tree.item(4 + 1, tags="red")
            if self.current_data[8] >= 1000:
                self.text_box.insert(END, "Ambient > 1000\n")
                self.tree.item(8 + 1, tags="red")
            if self.current_data[6] < 25000:
                self.text_box.insert(END, "Current < 25000\n")
                self.tree.item(8 + 1, tags="red")

def updateData(frame, newData):
    """
    Updates the treeview in the frame with data found in newData.
    :param frame: The Tkinter frame with a treeview to display the data.
    :param newData: List with numbers to be displayed.
    """
    frame.current_data = newData
    for i in range(len(measurementList)):
        frame.tree.set(i + 1, column="#1", value=newData[i])


def cmdStop():
    """Function used to send stop signal"""
    try:
        mySocket.sendto('s' * 35 + ' stop', SERVER_IP)
    except TypeError:
        mainFrame.complain('Not connected')


def cmdReset():
    try:
        mySocket.sendto('s' * 35 + ' reset', SERVER_IP)
    except TypeError:
        mainFrame.complain('Not connected')


def cmdTime():
    """Function used to send time in seconds pod needs to run for before it may brake"""
    try:
        seconds = int(mainFrame.entry.get())
        mySocket.sendto('s' * 35 + ' time ' + str(seconds), SERVER_IP)
    except ValueError:
        mainFrame.complain('That is not an acceptable number.')
    except TypeError:
        mainFrame.complain('Not connected')


def cmdCommand():
    """Function used to send commands to Pi"""
    try:
        command = mainFrame.cmd_entry.get()
        mainFrame.cmd_entry.delete(0, 'end')
        mySocket.sendto('s' * 35 + ' cmd ' + command)
    except TypeError:
        mainFrame.complain('Not connected')


def count():
    """Continuously receives data from client"""
    global SERVER_IP
    while True:
        unpacker = struct.Struct('! B B i i i i i i i I')
        (data, addr) = mySocket.recvfrom(unpacker.size)
        receivedList = unpacker.unpack(data)
        mainFrame.parent.title("512 Hyperloop Display Panel: Client Connected")
        SERVER_IP = addr
        updateData(mainFrame, receivedList)
        mainFrame.check_warnings()


if __name__ == "__main__":
    root = Tkinter.Tk()
    mainFrame = Frame(root)
    threading.Thread(target=count).start()
    root.mainloop()
