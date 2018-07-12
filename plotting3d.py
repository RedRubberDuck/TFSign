from matplotlib import pyplot
from mpl_toolkits.mplot3d import Axes3D
import random
import numpy as np
import cv2


import numpy as np

fig = pyplot.figure()
ax = fig.add_subplot(111, projection='3d')

sequence_containing_x_vals = list(range(0, 50))
sequence_containing_y_vals = list(range(0, 50))
sequence_containing_z_vals = list(range(0, 255))


x_vals = np.array([sequence_containing_x_vals]).transpose()
y_vals = np.array([sequence_containing_y_vals]).transpose()
z_vals = np.array([sequence_containing_z_vals]).transpose()

rangeRed1 = [0, 24]
rangeSat = [0, 255]
rangeV = [0, 255]


red1InferiorLimit = [0, 125, 70]
red1SuperiorLimit = [12, 255, 255]
point = []
color3 = []
for r in range(rangeRed1[0], rangeRed1[1], 1):
    for s in range(rangeSat[0], rangeSat[1], 10):
        for v in range(rangeV[0], rangeV[1], 10):
            if (red1InferiorLimit[0] <= r and red1SuperiorLimit[0] >= r and red1InferiorLimit[1] <= s and red1SuperiorLimit[1] >= s and red1InferiorLimit[2] <= v and red1SuperiorLimit[2] >= v):
                point.append([r, s, v])
            # color3.append(r+s+v)

color = point
# print(point)
# color = np.concatenate((x_vals, y_vals,
#                         z_vals), axis=1)
color = np.array([color], dtype='uint8')
color = cv2.cvtColor(color, cv2.COLOR_HSV2RGB)
color2 = (np.array(color[0].tolist(), dtype='float')/255.0)
point = np.array(point)

# print(color2)
# = np.array([[255, 0, 0], [0, 255, 0], [0, 0, 255]])

# random.shuffle(sequence_containing_x_vals)
# random.shuffle(sequence_containing_y_vals)
# random.shuffle(sequence_containing_z_vals)

ax.scatter(point[:, 0], point[:, 1],
           point[:, 2], s=100, facecolors=color2)
pyplot.show()
