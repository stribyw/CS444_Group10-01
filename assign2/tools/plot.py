import matplotlib.pyplot as plt

time_adds = []
time_dispatches = []
adds = []
dispatches = []

f = open('processed.txt', 'r')
for line in f:
	split = line.split()
	time = float(split[1][:-1])
	if split[3] == 'dispatching':
		dispatches.append(int(split[4]))
		time_dispatches.append(time)
	elif split[3] == 'adding':
		adds.append(int(split[4]))
		time_adds.append(time)	

plt.scatter(time_adds, adds, label='Adds', color='blue')
plt.scatter(time_dispatches, dispatches, label='Dispatches', color='red')
plt.ylabel('Request sector')
plt.xlabel('Time (sec)')
plt.title('Request sector vs time')
plt.legend()
plt.show()
