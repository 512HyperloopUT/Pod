import RPi.GPIO as GPIO
import serial

GPIO.setmode(GPIO.BCM)

GPIO.setup(0, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(1, GPIO.IN)

GPIO.setup(5, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(12, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(13, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(19, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(26, GPIO.OUT, initial=GPIO.LOW)

ser = serial.Serial("/dev/ttyACM0")
ser.baudrate = 115200

COMM_EOC = chr(242)

def readUART():
    print('waiting for COMM_EOC to start buffer')
    while True:
        r = ser.read(1)
        if r == COMM_EOC:
            break

    print('reading buffer')

    val = 0
    offs = [24, 16, 8, 0]

    for off in offs:
        curr = ser.read(1)
        val |= (curr[0] << off)

    ser.read(1)

    return val

def setRead(sensorID):
    GPIO.output(0, GPIO.LOW)
    print('resetting TM4C command')
    while GPIO.input(1) != GPIO.LOW:
        pass

    print('setting sensor pins')
    print('bit 0: ', '1' if (sensorID & 0x1) != 0 else '0')
    print('bit 1: ', '1' if (sensorID & 0x2) != 0 else '0')
    print('bit 2: ', '1' if (sensorID & 0x4) != 0 else '0')
    print('bit 3: ', '1' if (sensorID & 0x8) != 0 else '0')
    print('bit 4: ', '1' if (sensorID & 0x10) != 0 else '0')
    GPIO.output(5, GPIO.HIGH if (sensorID & 0x1) != 0 else GPIO.LOW)
    GPIO.output(12, GPIO.HIGH if (sensorID & 0x2) != 0 else GPIO.LOW)
    GPIO.output(13, GPIO.HIGH if (sensorID & 0x4) != 0 else GPIO.LOW)
    GPIO.output(19, GPIO.HIGH if (sensorID & 0x8) != 0 else GPIO.LOW)
    GPIO.output(26, GPIO.HIGH if (sensorID & 0x10) != 0 else GPIO.LOW)

    GPIO.output(0, GPIO.HIGH)

    print('writing for TM4C response')
    while GPIO.input(1) != GPIO.HIGH:
        pass

    return sensorID

while True:
    print('r = read, x = reset, q = quit')
    choice = input('choice: ')
    if choice == 'r':
        readID = input('readID: ')
        setRead(int(readID))
        print('readUART: ', readUART())
    elif choice == 'x':
        setRead(31)
    elif choice == 'q':
        break
    else:
        print('invalid option')

ser.close()
GPIO.cleanup()
