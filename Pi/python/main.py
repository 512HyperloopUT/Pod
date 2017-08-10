import RPi.GPIO as GPIO
import serial

GPIO.setmode(GPIO.BCM)

GPIO.setup(0, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(1, GPIO.IN)

GPIO.setup(5, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(12, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(13, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(19, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(23, GPIO.OUT, initial=GPIO.LOW)

#ser = serial.Serial("/dev/ttyACM0")
#ser.baudrate = 115200

COMM_EOC = chr(242)
'''
def readUART():
    print('waiting for COMM_EOC to start buffer')
    while ser.read(1) != COMM_EOC:
        pass

    print('reading buffer')

    val = 0
    offs = [24, 16, 8, 0]

    for off in offs:
        curr = ser.read(1)
        val |= (curr << off)

    return val
'''

def setRead(sensorID):
    GPIO.output(0, GPIO.LOW)
    print('resetting TM4C command')
    while GPIO.input(1) != GPIO.LOW:
        pass

    print('setting sensor pins')
    print('bit 0: ', '1' if (sensorID & 0x1) != 0 else '0')
    print('bit 1: ', '1' if (sensorID & 0x10) != 0 else '0')
    print('bit 2: ', '1' if (sensorID & 0x100) != 0 else '0')
    print('bit 3: ', '1' if (sensorID & 0x1000) != 0 else '0')
    print('bit 4: ', '1' if (sensorID & 0x10000) != 0 else '0')
    GPIO.output(5, GPIO.HIGH if (sensorID & 0x1) != 0 else GPIO.LOW)
    GPIO.output(12, GPIO.HIGH if (sensorID & 0x10) != 0 else GPIO.LOW)
    GPIO.output(13, GPIO.HIGH if (sensorID & 0x100) != 0 else GPIO.LOW)
    GPIO.output(19, GPIO.HIGH if (sensorID & 0x1000) != 0 else GPIO.LOW)
    GPIO.output(23, GPIO.HIGH if (sensorID & 0x10000) != 0 else GPIO.LOW)

    GPIO.output(0, GPIO.HIGH)

    print('writing for TM4C response')
    while GPIO.input(1) != GPIO.HIGH:
        pass

    return sensorID

setRead(5)
setRead(1)
setRead(2)

#ser.close()
GPIO.cleanup()
