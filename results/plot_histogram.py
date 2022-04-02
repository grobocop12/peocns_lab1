import matplotlib.pyplot as plt
import numpy as np

histogram1 = []

with open('histogram1.txt') as f:
    for line in f.readlines():
        histogram1.append(float(line.split(' ')[-1]) * 100.0)

histogram2 = []

with open('histogram2.txt') as f:
    for line in f.readlines():
        histogram2.append(float(line.split(' ')[-1]) * 100.0)

x = np.arange(0, len(histogram1))

hist = np.array([x, histogram1])

plt.bar(x, histogram1)
plt.xticks(np.arange(0,51,5))
plt.yticks(np.arange(0,51,5))
plt.title("Serwer 1")
plt.xlabel("Stan kolejki")
plt.ylabel('''% czasu symulacji''')
plt.grid()

plt.figure()
plt.bar(x, histogram2)
plt.xticks(np.arange(0,51,5))
plt.yticks(np.arange(0,51,5))
plt.title("Serwer 2")
plt.xlabel("Stan kolejki")
plt.ylabel('''% czasu symulacji''')
plt.grid()
plt.show()