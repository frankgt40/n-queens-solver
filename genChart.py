import matplotlib.pyplot as plt
import csv

x = []
y = []

with open('eval.txt','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        x.append(int(row[0]))
        y.append(int(row[1]))

plt.plot(x,y,'.', label='execution time for each n')
plt.xlabel('number of n')
plt.ylabel('time (nanoseconds)')
plt.title('Execution time of using explicit solution method')
plt.legend()
plt.show()
