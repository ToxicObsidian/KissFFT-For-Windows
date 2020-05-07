# This script is written by Toxic_Obsidian
# @ Support : python 3.x
# @ Libraries : numpy, matplotlib

import sys
import numpy as np
import matplotlib.pyplot as plt

def usage():
    print("Usage:")
    print("    DrawSpectrum.py file datatype audiotype")
    print("e.g.:")
    print("    DrawSpectrum.py .\\Samples\\.Info\\IEEE_FLOAT\\PureSine-Amaj_data.txt float chord")
    print("    DrawSpectrum.py .\\Samples\\.Info\\INT16\\PureSine-A4_data.txt int key")


wavtab = []
wavind = []

if(len(sys.argv) != 4):
    print("Too few or too many arguments: " + str(len(sys.argv)) + " argument(s)")
    usage()
    exit(0)

szDataFile = sys.argv[1]
dataType = sys.argv[2]
audioType = sys.argv[3]

if(szDataFile[-9:] != '_data.txt'):
    print('Invalid file name, use the file name with suffix `_data.txt`')
    exit(-1)
if(dataType != 'int' and dataType != 'float'):
    print('Invalid dataType: ' + dataType)
    exit(-1)
if(audioType != 'key' and audioType != 'chord'):
    print('Invalid audioType: ' + audioType)
    exit(-1)


with open(szDataFile, 'r') as pf:
    if dataType == 'float':
        wavtab = [float(content) for content in pf]
    elif dataType == 'int':
        wavtab = [int(content) for content in pf]


# Delete one of the channel data, use mono channel.
for index in range(len(wavtab) - 1, 0, -2):
    del wavtab[index]
wavind = range(0, len(wavtab))

plt.plot(wavtab)
if szDataFile[-10] == 'j':
    plt.title("Magnitude - Time figure (Pure Sine Wave Chord - Amaj)")
elif szDataFile[-11] == '#':
    plt.title("Magnitude - Time figure (Pure Sine Wave - C#5)")
else:
    plt.title("Magnitude - Time figure (Pure Sine Wave - {})".format(szDataFile[-11:-10]))
if dataType == 'float':
    if audioType == 'key':
        plt.ylim(-0.5, 0.5)
    elif audioType == 'chord':
        plt.ylim(-1, 1)
elif dataType == 'int':
    if audioType == 'key':
        plt.ylim(-10000, 10000)
    elif audioType == 'chord':
        plt.ylim(-30000, 30000)
plt.xlim(0, 1000)
plt.xlabel("Time (1 Oscillation = 1/44100s)", fontsize = 14)
plt.ylabel("Magnitude (PCM - INT16)", fontsize = 14)
plt.show()

