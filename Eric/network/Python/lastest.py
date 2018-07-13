from laspy.file import File
import numpy as np

inFile = File('C:/Users/OptiComp/Desktop/Eric/Python/Serpent Mound Model LAS Data.las', mode='r')
print('X.Y.Z:', inFile.x[1], inFile.y[1], inFile.z[1])

inFile.close()
