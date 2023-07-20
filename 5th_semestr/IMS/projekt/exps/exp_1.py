import os
import sys
from mpl_toolkits import mplot3d
import matplotlib.pyplot as plt
import numpy as np

from_workers = sys.argv[1]
to_workers = sys.argv[2]
from_robots = sys.argv[3]
to_robots = sys.argv[4]
best = int(sys.argv[5])

os.system('make clean')
os.system('make')

os.system('rm ./exps/exp_out_1.txt')

print('Running experiment number 1 ...')

for i in range(int(from_workers), int(to_workers) + 1):
	for y in range(int(from_robots), int(to_robots) + 1):
		os.system('./simulator -w ' + str(i) + ' -r ' + str(y) + ' >> exps/exp_out_1.txt')

print('Experiment number 1 done.')

f = open('./exps/exp_out_1.txt', 'r')

x_cord = []
y_cord = []
z_cord = []

for x in f:
	line = x.split(':')
	if line[0] == 'Workers':
		x_cord.append(int(line[1]))
	elif line[0] == 'Robots':
		y_cord.append(int(line[1]))
	elif line[0] == 'Time to sort all packages':
		z_cord.append(int(line[1]))

f.close()

fig = plt.figure(figsize = (8, 8))
ax = fig.add_subplot(111, projection = '3d')
ax.scatter(x_cord, y_cord, z_cord, c = z_cord, cmap = 'viridis', linewidth = 0.5)
ax.view_init(-140, 60)
ax.set_xlabel('pracovníci')
ax.set_ylabel('roboti')
ax.set_zlabel('doba třídění')

# plt.show()
plt.savefig('exps/exp_fig_1.png')

z_array = np.array(z_cord)
z_inds = np.argpartition(z_array, best)[:best]

res = 510
res_ind = 0

for i in z_inds:
	fit = (x_cord[i] * 2) + y_cord[i]
	if fit < res:
		res = fit
		res_ind = i

print('Best count of workers and robots.')
print('Workers: ', x_cord[i])
print('Robots: ', y_cord[i])
print('Time to sort all packages: ', z_cord[i], 'minutes')