import lib.hyper_comms as comms
import lib.hyper_periph as periph

comms.init()

while True:
    print('r = read, w = write, q = quit')
    choice = input("choice: ")
    if choice == 'r':
        readID = int(input("readID: "))
        print('read: ', comms.read(readID))
    elif choice == 'w':
        writeID = int(input("writeID: "))
        dir = comms.WriteDir(int(input("dir: ")))
        comms.write(writeID, dir)
    elif choice == 'q':
        break
    else:
        print('invalid option')

comms.free()
