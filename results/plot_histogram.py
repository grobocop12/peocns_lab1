import matplotlib.pyplot as plt
import numpy as np

histogram1 = []

with open('histogram1.txt') as f:
    for line in f.readlines():
        histogram1.append(float(line.split(' ')[-1]))

histogram2 = []

with open('histogram2.txt') as f:
    for line in f.readlines():
        histogram2.append(float(line.split(' ')[-1]))

x = np.arange(0, len(histogram1))
hist = np.array([x, histogram1])

plt.bar(x, histogram1)
plt.show()

plt.bar(x, histogram2)
plt.show()