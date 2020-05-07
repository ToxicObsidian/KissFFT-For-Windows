import sys
import numpy as np
import matplotlib.pyplot as plt

def usage():
    print('Usage: FFTUsingNumpy.py file channel_count datatype freq_of_sample [sample_count]')
    print('    If sample_count is not specified, use the full length of data')
    print('e.g.:')
    print('    FFTUsingNumpy.py ..\\Samples\\.Info\\IEEE_FLOAT\\PureSine-Amaj_data.txt 2 float 44100 44100')
    exit(0)

if(len(sys.argv) < 4):
    usage()

wavtab = []
szDataFile = sys.argv[1]
nChannels = int(sys.argv[2])
dataType = sys.argv[3]
fSampleFreq = float(sys.argv[4])
nSampleCount = -1
if(len(sys.argv) >= 6):
    nSampleCount = int(sys.argv[5])

with open(szDataFile, 'r') as pf:
    cnt = 0
    for content in pf:
        if(cnt % 2 == 0):
            wavtab.append(float(content)*1.0)
            if(dataType == 'int'):
                wavtab[-1] = wavtab[-1] / 25600.0
        cnt += 1
wavtab = wavtab[0 : nSampleCount]
print('Read data: {} samples'.format(len(wavtab)))
nSampleCount = len(wavtab)

plt.plot(wavtab)
plt.show()
Res = (np.fft.fft(wavtab) / nSampleCount) * 2
absRes = [np.abs(x) for x in Res]


i = 0
while i < len(absRes) // 2:
    if(absRes[i] > 0.12):
            print("Freq: {}, Magnitude: {}".format(i * fSampleFreq / nSampleCount, absRes[i]))
    i += 1


plt.plot(absRes)
plt.show()



