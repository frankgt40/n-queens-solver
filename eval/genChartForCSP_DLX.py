import matplotlib.pyplot as plt
import numpy as np

# Make dummy csv file for this example
from io import StringIO 
result_csv = StringIO(u"""
N, DLX,	Simple CSP
4, 219000, 34000
5, 222000, 21000
6, 376000, 24000
7, 697000, 157000
8, 1583000, 317000
9, 4929000, 1365000
10, 10595000, 5306000
11, 46350000, 27022000
12, 232875000, 132105000
13, 1256585000, 775167000
14, 7046682000, 4558501000
15, 48900167000, 46215225000
16, 329008092000, INF
17, 2292389210000, INF
""")

# Read in csv. Use names=True to also store column headers
per_data=np.genfromtxt(result_csv,delimiter=',',names=True)

# Loop over columns. Here I assume you have the x-data in the first column, so skip that one
for name in per_data.dtype.names[1:]:
    # Set the line's label to the column name
    plt.plot(per_data['xstuff'],per_data[name],label=name)

# Add a legend
plt.legend(loc=0)

plt.xlabel ('x stuff')
plt.ylabel ('y stuff')
plt.title('my test result')
plt.grid()
plt.show()
