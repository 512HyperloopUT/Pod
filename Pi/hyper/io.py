import time
import struct
import os
from hyper import quat, comms, udp
from lib import bno055
import spidev
from RPi import GPIO


class Input:
    def __init__(self):
        # input backends
        self.comm_port = comms.CommPort()
        self.imu = IMUSensor()
        self.udp_port = udp.UDPClient()

        self.voltmeter = Sensor(-1, comm_port)
        self.ammeter = Sensor(-1, comm_port)
        self.prox1 = Sensor(-1, comm_port)
        self.prox2 = Sensor(-1, comm_port)
        self.prox3 = Sensor(-1, comm_port)
        self.prox4 = Sensor(-1, comm_port)
        self.retro1 = Sensor(-1, comm_port)
        self.retro2 = Sensor(-1, comm_port)
        self.lf_lev_pot = Sensor(-1, comm_port)
        self.rf_lev_pot = Sensor(-1, comm_port)
        self.lb_lev_pot = Sensor(-1, comm_port)
        self.rb_lev_pot = Sensor(-1, comm_port)
        self.lf_lat_pot = Sensor(-1, comm_port)
        self.rf_lat_pot = Sensor(-1, comm_port)
        self.lb_lat_pot = Sensor(-1, comm_port)
        self.rb_lat_pot = Sensor(-1, comm_port)

        # useful inputs
        self.start_time = time.time()
        self.duration = 0

        self.emag_activated = False
        self.emag_timer = -1
        self.user_command = None

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

        self.accelerationX = 0
        self.accelerationY = 0
        self.accelerationZ = 0

        self.OriW = 0
        self.OriX = 0
        self.OriY = 0
        self.OriZ = 0

        self.voltage = 0
        self.amperage = 0
        self.lf_lev_extension = 0
        self.rf_lev_extension = 0
        self.lb_lev_extension = 0
        self.rb_lev_extension = 0
        self.lf_lat_extension = 0
        self.rf_lat_extension = 0
        self.lb_lat_extension = 0
        self.rb_lat_extension = 0

    def update(self):
        self.duration = time.time() - self.start_time
        self.accelerationX = self.imu.getAccX()
        self.accelerationY = self.imu.getAccY()
        self.accelerationZ = self.imu.getAccZ()

        self.OriW = self.imu.getOriW()
        self.OriX = self.imu.getOriX()
        self.OriY = self.imu.getAccY()
        self.OriZ = self.imu.getOriZ()

        self.udp_port.update()
        self.emag_activated = self.udp_port.ebrake
        self.emag_timer = self.udp_port.timer
        self.user_command = self.udp_port.user_command

        self.voltage = self.voltmeter.get()
        self.amperage = self.ammeter.get()
        self.lf_lev_extension = self.lf_lev_pot.get()
        self.rf_lev_extension = self.rf_lev_pot.get()
        self.lb_lev_extension = self.lb_lev_pot.get()
        self.rb_lev_extension = self.rb_lev_pot.get()
        self.lf_lat_extension = self.lf_lat_pot.get()
        self.rf_lat_extension = self.rf_lat_pot.get()
        self.lb_lat_extension = self.lb_lat_pot.get()
        self.rb_lat_extension = self.rb_lat_pot.get()

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
        if not self.comm_port.read(31) == 512:
            return False

        # calibrate sensors

        return True


class Sensor:
    def __init__(self, in_id, comm_port):
        self.in_id = in_id
        self.comm_port = comm_port
        self.value = 0

    def update(self):
        if self.in_id == -1:
            return -1
        self.value = self.comm_port.read(self.in_id)

    def get(self):
        return self.value


class IMUSensor:
    IMU_ACCEL_SMOOTHING = 10
    IMU_ORIEN_SMOOTHING = 0.7

    def __init__(self):
        self.bno = bno055.BNO055(serial_port='/dev/ttyAMA0', rst=18)
        if self.bno.begin() is not True:
            print("Error initializing device")
            exit()
        time.sleep(1)
        # TODO not sure about this line
        # self.bno.setExternalCrystalUse(True)

        self.initial_rot = quat.normalize(self.bno.read_quaternion())
        self.value = [(0.0, 0.0, 0.0), (0.0, 0.0, 0.0),
                      self.initial_rot, (0.0, 0.0, 0.0), time.time()]

    def update(self):
        self.value = self.value[0: 2] + [self.bno.read_quaternion(), self.bno.read_linear_acceleration(),
                                         time.time()] + self.value[2: 5]
        # self.value[3] = tuple([round(x, 1) for x in self.value[3]])

        delta_t = self.value[4] - self.value[7]

        # simple complimentary lowpass on orientation
        # TODO test efficacy
        self.value[2] = tuple([
            (prev * IMUSensor.IMU_ORIEN_SMOOTHING) +
            (curr * (1 - IMUSensor.IMU_ORIEN_SMOOTHING))
            for prev, curr in zip(self.value[5], self.value[2])
        ])
        # get the rotation from current coordinate system to original coordinate system
        rot_q = quat.q_mult(quat.q_conjugate(
            quat.normalize(self.value[2])), self.initial_rot)
        # Rotate relative, linear acceleration to initial orientation
        self.value[3] = quat.qv_mult(rot_q, self.value[3])
        # Do a lowpass on accel
        # TODO test efficacy
        self.value[3] = tuple([
            prev + (delta_t * (curr - prev) / IMUSensor.IMU_ACCEL_SMOOTHING)
            for prev, curr in zip(self.value[6], self.value[3])
        ])
        # TODO add in a filter here for accuracy sake
        # Add accel to velocity to get the current velocity
        self.value[1] = (
            self.value[1][0] + self.value[3][0],
            self.value[1][1] + self.value[3][1],
            self.value[1][2] + self.value[3][2]
        )
        # Do a simple time delta for the new distance traveled
        self.value[0] = (
            self.value[0][0] + (self.value[1][0] *
                                (self.value[4] - self.value[7])),
            self.value[0][1] + (self.value[1][0] *
                                (self.value[4] - self.value[7])),
            self.value[0][2] + (self.value[1][0] *
                                (self.value[4] - self.value[7]))
        )

    def data_string(self):
        return "\n\torientation: " + str(self.value[0]) + \
            "\n\tvelocity: " + str(self.value[1]) + \
            "\n\tcurrent orientation: " + str(self.value[2]) + \
            "\n\tcurrent accel: " + str(self.value[3]) + \
            "\n\tcurrent time: " + str(self.value[4])

    def getAccX(self):
        return self.value[3][0]

    def getAccY(self):
        return self.value[3][1]

    def getAccZ(self):
        return self.value[3][2]

    def getOriW(self):
        return self.value[2][0]

    def getOriX(self):
        return self.value[2][1]

    def getOriY(self):
        return self.value[2][2]

    def getOriZ(self):
        return self.value[2][3]


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

    def update(self):
        pass

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
