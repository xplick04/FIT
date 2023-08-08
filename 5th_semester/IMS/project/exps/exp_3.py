import os
import sys
from mpl_toolkits import mplot3d
import matplotlib.pyplot as plt
import numpy as np

from_pack = sys.argv[1]
to_pack = sys.argv[2]

os.system('make clean')
os.system('make')

os.system('rm ./exps/exp_out_3.txt')

print('Running experiment number 3 ...')

x_cord = []

for i in range(int(from_pack), int(to_pack) + 1, 200):
	os.system('./simulator -w 31 -r 49 -p ' + str(i) + ' >> exps/exp_out_3.txt')
	x_cord.append(i)

print('Experiment number 3 done.')

f = open('./exps/exp_out_3.txt', 'r')

y_cord = []

for x in f:
	line = x.split(':')
	if line[0] == 'Time to sort all packages':
		y_cord.append(int(line[1]))

f.close()

plt.plot(x_cord, y_cord)

plt.xlabel('počet balíků z minulé směny')
plt.ylabel('doba třídění')

plt.savefig('exps/exp_fig_3.png')
plt.show()