import serial, glob
import numpy as np
from matplotlib import pyplot as plt

NUM_SAMPLES = 5

port = glob.glob("/dev/ttyUSB*")[0]  #in case the device has walked down to another number


ser = serial.Serial(port)

data = []
for i in range(NUM_SAMPLES):
    ser.write("MEAS_PULSE_MICROS\n")
    line = ser.readline()
    val = int(line.strip())
    print "got value: %d" % val
    data.append(val)
    
data = np.array(data)

plot(data, ".-")

