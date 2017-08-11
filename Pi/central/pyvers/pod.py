#!/usr/bin/env python3
# Using python 3

import time

# Possibly could be wrong depending on pwd:
import pod_periph
import pod_spxgui


class Pod:
    def __init__(self):
        self.sensor_list = []
        self.actuator_list = []
        self.action_list = []
        self.client = pod_periph.Client()
        self.gui = pod_spxgui.Port()
        self.state = "idle"  # TODO figure out states

    def update_sensors(self):
        pass

    def update_actuators(self):
        pass

    def log(self):
        pass


class TimedActions:
    def __init__(self, sec: float, times: int):
        self.sec_per_call = sec
        self.reps = 0
        self.target_reps = times
        self.last = time.time()

    def act_time_check(self):
        temp = self.last
        last = time.time() - temp
        return last - temp > self.sec_per_call

    def check_act(self):
        # TODO check for max possible value of int64_t
        return True if self.target_reps is None else self.reps >= self.target_reps and self.act_time_check()

    def reset(self):
        reps = 0
        last = time.time()

    def do_action(self, ignore_check):
        if ignore_check or self.check_act():
            self.act(p)
            self.reps = self.reps + 1
            return True
        return False

    def act(self, p: Pod):
        print("Undefined action committed.")


def pre_init():
    # TODO
    pass


def init_pod(p: Pod):
    # TODO add sensors
    # TODO add actuators
    # TODO add actions
    # TODO client initialization
    # TODO GUI / UDP initialization
    pass


def pre_start(p: Pod):
    # TODO recover from stop / begin moving
    pass


def stop_check():
    # master_packet * packet = p.master.receive() TODO clear out the queue for the socket
    # TODO check for emergency stop
    # TODO check for other stops
    pass


def loop(p: Pod):
    p.update_sensors()
    # TODO insert processing here
    # TODO execute timed actions
    return stop_check()


def shutdown_check():
    # TODO check to see if should shutdown
    pass


def post_stop(p: Pod):
    # TODO cooldown after braking
    return shutdown_check()


def graceful_shutdown(p: Pod):
    # TODO, probably uninitialize
    pass


if __name__ == "__main__":
    p = Pod()
    pre_init()
    init_pod(p)
    shutdown = False
    while not shutdown:
        stop = pre_start(p)
        while not stop:
            stop = loop(p)
        shutdown = post_stop()
    graceful_shutdown()
