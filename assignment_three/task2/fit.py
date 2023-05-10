import sys
import numpy as np

datafile = sys.argv[1]
data = np.genfromtxt(datafile, delimiter=' ')[:,[0,-1]]
fit = np.polyfit(data[:,0], data[:,1], 1)
print(fit)