import time
import RPi.GPIO as GPIO
import lib.bno055 as bno055
import lib.hyper_quat as quat
import lib.hyper_comms as comms
import struct

class Input:
    def __init__(self):
        self.comms = comms.Comms()

        self.ebrake_requested = False
        self.ebrake_waittime = -1

        self.starttime = time.time()
        self.duration = 0

        # TODO: These values are so wrong and only for debug purposes, change later
        self.team_id = 1
        self.status = 2
        self.acceleration = 3
        self.position = 4
        self.velocity= 5
        self.battery_voltage = 6
        self.battery_current = 7
        self.battery_temperature = 8
        self.pod_temperature = 9
        self.stripe_count = 10

    def update(self):
        #check for ebrake on UDP
        self.duration = time.time() - self.starttime

    def get_packed_data(self):
        """
        :return: The 34 byte telemetry frame of required data as specified by the SpaceX documentation.
        """
        input_data = [self.team_id, self.status, self.acceleration, self.position, self.velocity, self.battery_voltage,
                      self.battery_current, self.battery_temperature, self.pod_temperature, self.stripe_count]
        packer = struct.Struct('! B B i i i i i i i I')
        packed_data = packer.pack(*input_data)
        return packed_data

class AnalogSensor():
    def __init__(self, id, comms):
        self.id = id
        self.comms = comms
        self.value = 0

    def update(self):
        self.value = self.comms.read(self.id)

    def data_string(self):
        return "sensor " + self.id + ": " + self.value


class IMUSensor():
    def __init__(self, id, comms):
        self.id = id
        self.comms = comms
        self.bno = bno055.BNO055(serial_port='/dev/ttyAMA0', rst=18)
        if self.bno.begin() is not True:
            print("Error initializing device")
            exit()
        time.sleep(1)
        # TODO not sure about this line
        # self.bno.setExternalCrystalUse(True)

        self.initial_rot = quat.normalize(self.bno.read_quaternion())
        self.value = [(0.0, 0.0, 0.0), (0.0, 0.0, 0.0), self.initial_rot, (0.0, 0.0, 0.0), time.time()]

    def update(self):
        self.value = self.value[0:2] + [self.bno.read_quaternion(), self.bno.read_linear_acceleration(),
                                        time.time()] + self.value[2:5]
        # self.value[3] = tuple([round(x, 1) for x in self.value[3]])

        delta_t = self.value[4] - self.value[7]

        # Do a lowpass on accel
        # TODO test efficacy
        self.value[3] = tuple([
            self.value[6][ind] + (
                delta_t * (self.value[3][ind] - self.value[6][ind]) / IMUSensor.IMU_ACCEL_SMOOTHING
            ) for ind in range(3)
        ])
        # simple complimentary lowpass on orientation
        # TODO test efficacy
        self.value[3] = tuple([
            (self.value[5][ind] * IMUSensor.IMU_ORIEN_SMOOTHING) +
            (self.value[2][ind] * (1 - IMUSensor.IMU_ORIEN_SMOOTHING))
            for ind in range(4)
        ])

        # get the rotation from current coordinate system to original coordinate system
        rot_q = quat.q_mult(quat.q_conjugate(quat.normalize(self.value[2])), self.initial_rot)
        # Rotate relative, linear acceleration to initial orientation
        final_accel = quat.qv_mult(rot_q, self.value[3])
        # Add accel to velocity to get the current velocity
        self.value[1] = (
            self.value[1][0] + final_accel[0],
            self.value[1][1] + final_accel[1],
            self.value[1][2] + final_accel[2]
        )
        # Do a simple time delta for the new distance traveled
        self.value[0] = (
            self.value[0][0] + (self.value[1][0] * (self.value[4] - self.value[7])),
            self.value[0][1] + (self.value[1][0] * (self.value[4] - self.value[7])),
            self.value[0][2] + (self.value[1][0] * (self.value[4] - self.value[7]))
        )

    def data_string(self):
        return self.name + \
               "\n\torientation: " + str(self.value[0]) + \
               "\n\tvelocity: " + str(self.value[1]) + \
               "\n\tcurrent orientation: " + str(self.value[2]) + \
               "\n\tcurrent accel: " + str(self.value[3]) + \
               "\n\tcurrent time: " + str(self.value[4])

class Actuator:
    def __init__(self, name, id, comms):
        self.name = name
        self.id = id
        self.comms = commms

    def set(self, val):
        self.comms.write(self.id, val)
