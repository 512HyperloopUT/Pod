import time
import struct
import os
from hyper import comms, udp
from lib import bno055, quat
import spidev
from RPi import GPIO


class Input:
    def __init__(self):
        # input backends
        self.comm_port = comms.CommPort()
        self.udp_port = udp.UDPClient()
        self.start_time = int(time.time() * 1000)

        self.voltmeter = AnalogSensor(19, self.comm_port)
        self.ammeter_highcurrent = AnalogSensor(18, self.comm_port)
        self.ammeter_lowcurrent = AnalogSensor(-1, self.comm_port)
        self.prox1 = DigitalSensor(0, self.comm_port)
        self.prox2 = DigitalSensor(1, self.comm_port)
        self.prox3 = DigitalSensor(2, self.comm_port)
        self.prox4 = DigitalSensor(3, self.comm_port)
        self.retro1 = DigitalSensor(4, self.comm_port)
        self.retro2 = DigitalSensor(5, self.comm_port)
        self.lf_lev_pot = AnalogSensor(3, self.comm_port)
        self.rf_lev_pot = AnalogSensor(2, self.comm_port)
        self.lb_lev_pot = AnalogSensor(1, self.comm_port)
        self.rb_lev_pot = AnalogSensor(0, self.comm_port)
        self.lf_lat_pot = AnalogSensor(9, self.comm_port)
        self.rf_lat_pot = AnalogSensor(11, self.comm_port)
        self.lb_lat_pot = AnalogSensor(16, self.comm_port)
        self.rb_lat_pot = AnalogSensor(17, self.comm_port)
        self.l_ebr_pot = AnalogSensor(10, self.comm_port)
        self.r_ebr_pot = AnalogSensor(9, self.comm_port)

        self.team_id = 1
        self.status = 2
        self.acceleration = 3  # cm/s^2
        self.position = 4  # cm
        self.velocity = 5  # cm/s
        self.battery_voltage = 6  # millivolts
        self.battery_current = 7  # milliamps
        self.battery_temperature = 8  # tenths of C
        self.pod_temperature = 9  # tenths of C
        self.stripe_count = 10

        # sensor inputs
        self.duration = 0

        self.emag_activated = False
        self.emag_timer = -1
        self.user_command = None

        self.voltage = 0
        self.amperage_highcurrent = 0
        self.amperage_lowcurrent = 0
        self.lf_lev_extension = 0
        self.rf_lev_extension = 0
        self.lb_lev_extension = 0
        self.rb_lev_extension = 0
        self.lf_lat_extension = 0
        self.rf_lat_extension = 0
        self.lb_lat_extension = 0
        self.rb_lat_extension = 0
        self.l_ebr_extension = 0
        self.r_ebr_extension = 0

        # outputs
        self.lev_extended = False
        self.lat_extended = False

    def updateFTA(self):
        self.lev_extended = False
        self.lat_extended = False
        pass

    def updateFTB(self):
        pass

    def updateFTC(self):
        pass

    def update(self):

        # SENSOR DATA
        self.voltage = self.voltmeter.get()
        self.amperage_highcurrent = self.ammeter_highcurrent.get()
        self.amperage_lowcurrent = self.ammeter_lowcurrent.get()
        self.lf_lev_extension = self.lf_lev_pot.get()
        self.rf_lev_extension = self.rf_lev_pot.get()
        self.lb_lev_extension = self.lb_lev_pot.get()
        self.rb_lev_extension = self.rb_lev_pot.get()
        self.lf_lat_extension = self.lf_lat_pot.get()
        self.rf_lat_extension = self.rf_lat_pot.get()
        self.lb_lat_extension = self.lb_lat_pot.get()
        self.rb_lat_extension = self.rb_lat_pot.get()
        self.l_ebr_extension = self.l_ebr_pot.get()
        self.r_ebr_extension = self.r_ebr_pot.get()

        # processing of data to be sent to GUI and SpaceX TODO: add other variable
        self.battery_voltage = self.voltage * 1000
        self.battery_current = self.amperage_highcurrent * 1000

        # CONTROL VARIABLES
        self.duration = int(time.time() * 1000) - self.start_time

        self.udp_port.update()
        self.emag_activated = self.udp_port.ebrake
        self.emag_timer = self.udp_port.timer
        self.user_command = self.udp_port.user_command

        self.updateFTA()
        # self.updateFTB()
        # self.updateFTC()
        # return

    def get_packed_data(self):
        """
        :return: The 34 byte telemetry frame of required data as specified by the SpaceX documentation.
        """
        input_data = [self.team_id, self.status, self.acceleration, self.position, self.velocity, self.battery_voltage,
                      self.battery_current, self.battery_temperature, self.pod_temperature, self.stripe_count]
        packer = struct.Struct('! B B i i i i i i i I')
        packed_data = packer.pack(*input_data)
        return packed_data

    def isReady(self):
        # if input is ready to be updated
        # check comms
        if not self.comm_port.readAnalog(31) == 512:
            return False

        # calibrate sensors

        return True


class DigitalSensor:
    def __init__(self, in_id, comm_port):
        self.in_id = in_id
        self.comm_port = comm_port

    def get(self):
        if self.in_id == -1:
            return -1
        return self.comm_port.readDigital(self.in_id)


class AnalogSensor:
    def __init__(self, in_id, comm_port):
        self.in_id = in_id
        self.comm_port = comm_port

    def get(self):
        if self.in_id == -1:
            return -1
        return self.comm_port.readAnalog(self.in_id)


class Voltmeter:
    def __init__(self, in_id, comm_port):
        self.in_id = in_id
        self.comm_port = comm_port

    def get(self):
        if self.in_id == -1:
            self.value = -1
            return
        val = self.comm_port.readAnalog(self.in_id)
        vout = (val * 5.0) / 1024.0
        vin = vout / (7500 / (30000 + 7500))
        vin = vin * .175
        return vin


class ThermoSensor:
    THERMO_PIN = 17

    def __init__(self, serial):
        self.serial = serial
        GPIO.setup(self.THERMO_PIN, GPIO.OUT, initial=GPIO.LOW)

        os.system('modprobe w1-gpio')
        os.system('modprobe w1-therm')
        self.temp_sensor = "sys/bus/w1/devices/28-" + self.serial + "/w1_slave"

        self.value = None

    def temp_raw(self):
        f = open(self.temp_sensor, 'r')
        lines = f.readlines()
        f.close()
        return lines

    def data_string(self):
        return "spi sensor, degrees celsius: " + str(self.value)


class DigitalOutput:
    def __init__(self, out_id, comm_port):
        self.out_id = out_id
        self.comm_port = comm_port

    def set(self, val):
        if self.out_id == -1:
            return
        self.comm_port.writeDigial(self.out_id, val)


class Actuator:
    def __init__(self, out_id, comm_port):
        self.out_id = out_id
        self.comm_port = comm_port

    def set(self, val):
        if self.out_id == -1:
            return
        self.comm_port.writeDigitalActuator(self.out_id, val)
