#!/usr/bin/env python
# coding: utf-8

import matplotlib.pyplot as plt
import numpy as np

data = np.loadtxt("data/data.txt")

data_n0, data_n1, data_n2 = {}, {}, {}
for i in range(len(data)):
    if (data[i][0] == 1000):
        data_n0[data[i][1]] = data[i][2]
    elif (data[i][0] == 1000000):
        data_n1[data[i][1]] = data[i][2]
    elif (data[i][0] == 100000000):
        data_n2[data[i][1]] = data[i][2]

fig = plt.figure()
ax = fig.add_subplot()
ax.set_title('N=1.000')
ax.set_xlabel('Число процессов')
ax.set_ylabel('Ускорение')
plt.scatter(data_n0.keys(), data_n0.values())
plt.plot(data_n0.keys(), data_n0.values())
plt.savefig('pics/N_1000.png')

fig = plt.figure()
ax = fig.add_subplot()
ax.set_title('N = 1.000.000')
ax.set_xlabel('Число процессов')
ax.set_ylabel('Ускорение')
plt.scatter(data_n1.keys(), data_n1.values())
plt.plot(data_n1.keys(), data_n1.values())
plt.savefig('pics/N_1_000_000.png')

fig = plt.figure()
ax = fig.add_subplot()
ax.set_xlabel('Число процессов')
ax.set_ylabel('Ускорение')
ax.set_title('N = 100.000.000')
plt.scatter(data_n2.keys(), data_n2.values())
plt.plot(data_n2.keys(), data_n2.values())
plt.savefig('pics/N_100_000_000.png')


