

"""
    msd=[]
    for time lag in positions.shape[0]:
        slice
        pos1=positions[timelag:,:,:]
        pos2=positions[:-timelag,:,:]
        diff=pos2-pos1
        diff=diff**2
        diff=diff.sum(3)
        diff=diff.mean
        msd.append()
"""

import os
from tqdm import tqdm
import gzip
import numpy as np

folder_path=r"D:\GP\3D ISA\python\positions"
files = os.listdir(folder_path)
print(files)
Positions=[]
for index,file in tqdm(enumerate(files)) :
    file_path=os.path.join(folder_path,file)

    file = gzip.GzipFile(file_path , "r")  
    # read the data and append it in the global variable with shape (3 , height * width * depth)         
    position = np.load(file , allow_pickle = True)
    
    Positions.append(position)
msd=[]
print(np.shape(Positions))
print(np.isnan(Positions))


for time_lag in range( np.shape(Positions)[0]-1):
    pos1=Positions[time_lag+1:]
    pos2=Positions[:-time_lag-1]
    diff=np.array(pos2)-np.array(pos1)
    diff=diff**2
    diff=np.array(diff).sum(1)
    d=diff.mean(0)
    msd.append(d)


for i in range(np.shape(msd)[0]):
    msd[i] = msd[i][~np.isnan(msd[i])]

print(np.shape(msd[i]))
print(np.linspace(1,np.shape(msd)[0],19))
import matplotlib.pyplot as plt
plt.plot(np.linspace(1,np.shape(msd)[0],19),msd)
plt.show()


