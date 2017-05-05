import matplotlib.pyplot as plt

adds = []
dispatches = []

f = open('processed.txt', 'r')
for line in f:
	split = line.split()
	if split[3] == 'dispatching':
		dispatches.append(int(split[4]))
	elif split[3] == 'adding':
		adds.append(int(split[4]))	

plt.plot(adds, label='Adds')
plt.plot(dispatches, label='Dispatches')
plt.ylabel('Request sector')
plt.title('Request sector vs time')
plt.legend()
plt.show()
