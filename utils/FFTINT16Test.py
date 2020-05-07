import numpy as np
import matplotlib.pyplot as plt
import random as rd

fSampleFreq = 44100.0
nSampleCount = 9600

signal = [32767 * np.cos(2 * np.pi * 440 * (t / fSampleFreq) - np.pi * 90 / 180.0) +
          32767 * np.cos(2 * np.pi * 659 * (t / fSampleFreq) - np.pi * 90 / 180.0) +
          32767 * np.cos(2 * np.pi * 552 * (t / fSampleFreq) - np.pi * 90 / 180.0) +
          rd.randint(-5000, 5000) * 1.0
          for t in range(0, nSampleCount)]
mx = max(signal)
signal = [x * 32767 / (mx * 1.0) for x in signal]
for ind in range(len(signal)):
    signal[ind] = int(signal[ind])

plt.plot(signal[0: 1000])
plt.show()

Res = np.fft.fft(signal) / nSampleCount * 2
absRes = [np.abs(x) for x in Res]


plt.plot(absRes[0: 2000])
plt.show()

for i in range(nSampleCount // 2):
    if(absRes[i] > 5000):
        print("freq: {}, Magnitude: {}".format(i * fSampleFreq / nSampleCount, absRes[i]))
