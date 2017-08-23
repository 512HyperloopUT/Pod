import lib.hyper_comms as comms

while True:
    print('r = read, w = write, q = quit')
    choice = input("choice: ")
    if choice == 'r':
        readID = input("readID: ")
        print('readUART: ', comms.read(readID))
    elif choice == 'w':
        writeID = input("writeID: ")
        dir = WriteDir(int(input("dir: ")))
        comms.write(writeID, dir)
    elif choice == 'q':
        break
    else:
        print('invalid option')

comms.free()
