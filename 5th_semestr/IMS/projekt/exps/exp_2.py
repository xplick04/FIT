import os
import sys
from mpl_toolkits import mplot3d
import matplotlib.pyplot as plt
import numpy as np

from_cars = sys.argv[1]
to_cars = sys.argv[2]

os.system('make clean')
os.system('make')

os.system('rm ./exps/exp_out_2.txt')

print('Running experiment number 2 ...')

x_cord = []

for i in range(int(from_cars), int(to_cars) + 1):
	os.system('./simulator -w 35 -c ' + str(i) + ' >> exps/exp_out_2.txt')
	x_cord.append(i)

print('Experiment number 1 done.')

f = open('./exps/exp_out_2.txt', 'r')

vals_i = []
vals_s = []

for x in f:
	line = x.split(':')
	if line[0] == 'Number of incoming packages':
		vals_i.append(int(line[1]))
	elif line[0] == 'Number of sorted packages':
		vals_s.append(int(line[1]))

f.close()

plt.plot(x_cord, vals_i, label = 'příchozí')
plt.plot(x_cord, vals_s, label = 'roztříděné')

plt.xlabel('počet aut')
plt.ylabel('počet balíků')

plt.legend()

# plt.show()
plt.savefig('exps/exp_fig_2.png')