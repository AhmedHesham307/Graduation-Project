import numpy as np
import os
from tqdm import tqdm
import gzip
import pickle as pk
import pycuda.gpuarray as gpuarray
from pycuda.compiler import SourceModule
import pycuda.autoinit
import imageio
import pickle as pk
#from Farneback_3dOpticalFlow import Farneback_3d
#from Visualizer import visualizer 
import scipy.ndimage 

folder_path=r".\fields_out"
files = sorted(os.listdir(folder_path))
print(files)

first_volume=imageio.imread(r'.\heart_LV_ch01_100_res2.tiff')[0]

#init positions
height=74
width=82
depth=83
dim=np.array([ height, width, depth])
dim_gpu=gpuarray.to_gpu(dim)
N=int(height*width*depth)
poistions_folderpath=r".\positions_out"
#x=np.linspace(0 , height - 1 , height)
#y=np.linspace(0 ,  width - 1 ,  width)
#z=np.linspace(0 ,  depth - 1 ,  depth)
#X, Y, Z = np.meshgrid(x,y,z,indexing='ij')
#X=X.flatten().astype(np.float32)
#Y=Y.flatten().astype(np.float32)
#Z=Z.flatten().astype(np.float32)
points = np.where(first_volume != 0 )

points=np.asarray(points,dtype=np.float32)
#points=np.array([X,Y,Z])

N=points.shape[1]

with gzip.open(os.path.join( poistions_folderpath,"000.gz"), "wb") as f:
    pk.dump(points, f)
position = gpuarray.to_gpu(points)
next_position= position.copy()
interpolated_flow = gpuarray.GPUArray( N, np.float32)

#kernel
with open(os.path.join(os.path.dirname(__file__), 'trilinear_interpolation.cu')) as f:
    read_data = f.read()
f.closed
blocksize=int(1024)
gridsize=int(np.ceil( N/blocksize))
block = (blocksize,1,1)
grid = (gridsize,1)

mod = SourceModule(read_data)
trilinear_interpolation_kernel = mod.get_function('trilinear_interpolation')
for index,file in tqdm(enumerate(files)) :
    file_path=os.path.join(folder_path,file)
    with gzip.open(file_path, 'rb') as f:
      field = pk.load(f)
    for i in range(3):
        flow=gpuarray.to_gpu(field[i])
        trilinear_interpolation_kernel(
                          np.int32( N),
                          np.int32(i),
                          dim_gpu,
                          position[0],
                          position[1],
                          position[2],
                          interpolated_flow,
                          flow,
                          block= block,
                          grid =  grid)
        next_position[i]= interpolated_flow
    position= next_position
    with gzip.open(os.path.join( poistions_folderpath,str(str(int((index+1)/100))+str(int((index+1)/10))+str((index+1)%10)+".gz")), "wb") as f:
      pk.dump( next_position.get(), f)