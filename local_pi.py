from Pi.hyper.io import *
from tkinter import *
from tkinter.ttk import *
import threading
import time

measurementList = ["Current"]
defaultValues = "0" * len(measurementList)
input_pi = Input()

class ServerFrame(Frame):
    """
    Singleton GUI frame
    """
    def __init__(self, parent):
        Tk.frame.__init__(self, parent)
        s = Style()
        s.theme_use('default')
        self.parent = parent
        self.client_tree = Treeview(parent, columns='tree')
        self.command_box = Entry(parent)
        self.command_button = Button(parent, text="Send", command=send_command)

        self.parent.title("Debug Panel")

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

def update_data():
    while True:
        data_list = [input_pi.battery_current]
        for i in range(len(measurementList)):
            main_frame.client_tree.set(i + 1, column="#1", value=data_list[i])
        time.sleep(.5)

if __name__ == "__main__":
    threading.Thread(target=update_data).start()
    root = Tk()
    ServerFrame.root = root
    main_frame = ServerFrame(root)
    main_frame.parent.mainloop()
