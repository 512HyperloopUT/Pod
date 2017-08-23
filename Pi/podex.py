import lib.hyper_comms as comms
import lib.hyper_pod as pod

comms.init()

pod = pod.Pod()
pod.start()

comms.free()
