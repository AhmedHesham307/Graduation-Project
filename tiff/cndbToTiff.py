from OpenMiChroM.CndbTools import cndbTools
import numpy as np
from sklearn.preprocessing import MinMaxScaler
import math
import tifffile as tif
import os

#from pathlib import Path


def scale_data(data, dimension):
    scaler = MinMaxScaler(feature_range=(0,dimension-1))
    scaler.fit(data)
    scaled_data = scaler.transform(data)
    scaled_data = np.around(scaled_data)
    scaled_data = scaled_data.astype(int)
    return scaled_data

cndbPath = './cndb_in'


x_max, x_min, y_max, y_min, z_max, z_min = 0.0,0.0,0.0,0.0,0.0,0.0


for cndb in os.scandir(cndbPath):
    print(cndb)
    if cndb.is_file():
        cndbObj = cndbTools()
        cndbObj.load(cndb)
        xyz = cndbObj.xyz(frames=[1,None,1], beadSelection='all', XYZ=[0,1,2])
        x_max_temp = np.max(xyz[:,:,0])
        y_max_temp = np.max(xyz[:,:,1])
        z_max_temp = np.max(xyz[:,:,2])

        x_min_temp = np.min(xyz[:,:,0])
        y_min_temp = np.min(xyz[:,:,1])
        z_min_temp = np.min(xyz[:,:,2])
        if(x_max_temp > x_max):
            x_max = x_max_temp
        if(y_max_temp > y_max):
            y_max = y_max_temp
        if(z_max_temp > z_max):
            z_max = z_max_temp

        if(x_min_temp < x_min):
            x_min = x_min_temp
        if(y_min_temp < y_min):
            y_min = y_min_temp
        if(z_min_temp < z_min):
            z_min = z_min_temp
        
        
            

# conc = np.concatenate((heart_LV_chr01_xyz,heart_LV_chr02_xyz), axis=1)

# Dimensions in nano-meters 
width = x_max - x_min
height = y_max - y_min
depth = z_max - z_min

# Define the resolution of the grid 
resolution = 2
grid_width = math.ceil(resolution * width)
grid_height= math.ceil(resolution * height)
grid_depth = math.ceil(resolution * depth)
print(grid_width)
print(grid_height)
print(grid_depth)



array_3d = np.zeros((100,grid_width, grid_height, grid_depth),dtype=np.uint8)

r=0
# array_3d = np.zeros((100,grid_width+ (r*2), grid_height+ (r*2), grid_depth+ (r*2)),dtype=np.uint8)

for cndb in os.scandir(cndbPath):
    print(cndb)
    if cndb.is_file():
        cndbObj = cndbTools()
        cndbObj.load(cndb)
        xyz = cndbObj.xyz(frames=[1,None,1], beadSelection='all', XYZ=[0,1,2])
        xyz[:,:,0] = scale_data(xyz[:,:,0], grid_width)
        xyz[:,:,1] = scale_data(xyz[:,:,1], grid_height)
        xyz[:,:,2] = scale_data(xyz[:,:,2], grid_depth)
        for frame in range(100):#(conc.shape[0]):
            for point in xyz[frame]:
                # Get the corresponding voxel for the point cloud coordinates 
                voxel_x = int(point[0])+r
                voxel_y = int(point[1])+r
                voxel_z = int(point[2])+r
                array_3d[frame][voxel_x][voxel_y][voxel_z] += 100
                
                #array_3d[frame, voxel_x-r : voxel_x+r, voxel_y-r: voxel_y+r, voxel_z-r:voxel_z+r] = 100

            print("frame: " + str(frame))
        
image = array_3d
tif.imsave("out.tiff", image, compression ='zlib')