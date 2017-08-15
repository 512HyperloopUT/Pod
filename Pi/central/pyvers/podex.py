#!/usr/bin/env python3
# Using python 3

import time

# Possibly could be wrong depending on pwd:
import pod_periph
import pod_spxgui

IDLE_STATE = "idle"
IS_DEBUG = True


class Pod:
    def __init__(self):
        self.sensor_list = []
        self.actuator_list = []
        self.action_list = []

        self.data_list = []

        self.client = pod_periph.Client()
        self.gui = pod_spxgui.Port()
        self.state = IDLE_STATE  # TODO figure out states

        self.start = None
        self.brake_disabled = 0
        self.can_brake = None

        # TODO create logfile

    def update_sensors(self):
        for sensor in self.sensor_list:
            sensor.update_sensor(self.client)

    def update_actuators(self):
        # TODO default loop logic here
        pass

    def update_actions(self):
        for action in self.action_list:
            action.do_action()

    def log(self):
        # TODO append data to log
        pass

    def start(self):
        self.start = time.time()
        self.can_brake = self.start + self.brake_disabled

    def set_boundary(self, req_time_elapsed):
        self.brake_disabled = req_time_elapsed
        if not self.start == None:
            self.can_brake = self.start + self.brake_disabled

    def brake(self):
        if self.can_brake > time.time():
            self.start = None
            self.can_brake = None
            # TODO brake procedures
        else:
            # TODO check if do nothing or do something
            pass


class TimedAction:
    def __init__(self, sec: float, times: int = -1):
        self.sec_per_call = sec
        self.reps = 0
        self.target_reps = times
        self.last = time.time()

    def act_time_check(self):
        temp = self.last
        self.last = time.time() - temp
        return self.last - temp > self.sec_per_call

    def check_act(self):
        return (True if self.target_reps == -1 else self.reps >= self.target_reps) and self.act_time_check()

    def reset(self):
        self.reps = 0
        self.last = time.time()

    def do_action(self, ignore_check: bool = False):
        if ignore_check or self.check_act():
            self.act(p)
            self.reps = self.reps + 1
            return True
        return False

    def act(self, pod_instance: Pod):
        print("Undefined action committed.")


class UpdateAction(TimedAction):
    def act(self, pod_instance: Pod):
        pod_instance.gui.send([])  # TODO what data


class LoggerAction(TimedAction):
    def act(self, pod_instance: Pod):
        pod_instance.log()


def pre_init():
    print("""
                        ,:
    512 Hyperloop     ,' |
                     /   :
  ,MMM8&&&.        --'   /
  MMMM88&&&&&      \/ />/
 MMMM88&&&&&&&     / /_\\
 MMM88&&&&&&&&  __/   /
 MMM88&&&&&&&&  )'-. /
 'MMM88&&&&&&'   ./  :\\
   'MMM8&&&'     /.' '
               '/'
 Running     .-"-
  podex    (    |  Art credits to anon
        . .-'  '.
        ( (.   )8:
     .'    / (_  )
    _. :(.   )8P  `""")


def init_pod(pod_instance: Pod):
    # TODO add sensors

    # TODO add actuators

    # TODO add actions
    gui_update_action = UpdateAction(0.02)
    pod_instance.action_list.append(gui_update_action)
    log_action = LoggerAction(0.02)
    pod_instance.action_list.append(log_action)

    # Peripheral initialization
    pod_instance.client = pod_periph.Client()
    # GUI / UDP initialization
    pod_instance.gui = pod_spxgui.Port(input("Server IP: "))
    pass


def pre_start(pod_instance: Pod):
    # TODO recover from stop / begin moving
    return stop_check(pod_instance)


def stop_check(pod_instance: Pod):
    # TODO clear out the queue for the socket
    # TODO check for emergency stop
    load = pod_instance.gui.receive()
    seconds = load.split()
    if load.startswith('s' * 35):
        if len(seconds) == 1:
            if IS_DEBUG:
                print("EMERGENCY STOP")
            # TODO: handle stop
            return True
        else:
            if IS_DEBUG:
                print(seconds[1])
            if seconds[1] == 'reset':  # If a reset is triggered on the GUI
                pod_instance.client.reset()
            else:  # If a time update is given on the GUI, number sent is contained in seconds[1]
                pod_instance.set_boundary(int(seconds[1]))
                pass
    else:
        print(*seconds)

    # TODO check for other stops
    return False


def loop(pod_instance: Pod):
    pod_instance.update_sensors()
    pod_instance.update_actuators()
    pod_instance.update_actions()
    return stop_check(pod_instance)


def shutdown_check(pod_instance: Pod):
    # TODO check to see if should shutdown
    return False


def post_stop(pod_instance: Pod):
    if pod_instance.state == IDLE_STATE:
        pass
    else:
        pod_instance.brake()
    # TODO after braking
    return shutdown_check(pod_instance)


def graceful_shutdown(pod_instance: Pod):
    # TODO, probably uninitialize components
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
        shutdown = post_stop(p)
    graceful_shutdown(p)
