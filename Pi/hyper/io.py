import time

from hyper import quat, comms
from lib import bno055


class Input:
    def __init__(self):
        self.comm_port = comms.CommPort()

        self.ebrake_requested = False
        self.ebrake_wait_time = -1

        self.start_time = time.time()
        self.duration = 0

        self.team_id = 1
        self.status = 2
        self.acceleration = 3

    def update(self):
        # check for ebrake on UDP
        self.duration = time.time() - self.start_time


class Sensor:
    def __init__(self, name, sensor_id):
        self.name = name
        self.sensor_id = sensor_id


class AnalogSensor(Sensor):
    def __init__(self, name, sensor_id, comm_port):
        super().__init__(name, sensor_id)
        self.comm_port = comm_port
        self.value = 0

    def update(self):
        self.value = self.comm_port.read(self.sensor_id)

    def data_string(self):
        return "sensor " + self.sensor_id + ": " + str(self.value)


class IMUSensor(Sensor):
    IMU_ACCEL_SMOOTHING = 10
    IMU_ORIEN_SMOOTHING = 0.7

    def __init__(self, name, sensor_id):
        super().__init__(name, sensor_id)
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
        # TODO add in a filter here for accuracy sake
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
    def __init__(self, name, actu_id, comm_port):
        self.name = name
        self.actu_id = actu_id
        self.comm_port = comm_port

    def set(self, val):
        self.comm_port.write(self.actu_id, val)