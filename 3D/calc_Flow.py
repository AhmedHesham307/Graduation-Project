import numpy as np
import scipy.ndimage as sciimg
#import farneback3d
import os
from tqdm import tqdm
import pandas as pd
import gzip
import pickle as pk
import opticalflow3D
import tifffile as tif
import opticalflow3D


class Optical_FLow:
  def __init__(self, path):
    self.volumes = tif.imread(path)[:20]
    self.number_of_volumes,self.height,self.width,self.depth = np.shape(self.volumes)
    
    self.fields_folderpath = r".\fields_out"

    self.optflow = opticalflow3D.Farneback3D(iters=1,
                                      num_levels=5,
                                      scale=0.8,
                                      spatial_size=3,
                                      presmoothing=3,
                                      filter_type="box",
                                      filter_size=9,
                                     )

  def optical_flow(self):
    vol1 = self.volumes[0]
    # for is implemented to run each and every frame in a loop
    for i in tqdm(range(self.number_of_volumes-1)):
        vol2 = self.volumes[i+1] #.astype(np.float32)
        # calculate frame-to-frame flow between vol0 and vol1
        shape = vol1.shape
        
        self.vz, self.vy, self.vx, _  = self.optflow.calculate_flow(vol1, vol2,total_vol=shape)
        self.vz[np.isnan(self.vz)] = 0
        self.vy[np.isnan(self.vy)] = 0
        self.vx[np.isnan(self.vx)] = 0
        self.displacements = [self.vx, self.vy, self.vz]
        with gzip.open(os.path.join(os.path.join(self.fields_folderpath,str(str(int(i/100))+str(int(i/10))+str(i%10)+".gz"))), "wb") as f:
          pk.dump(np.asarray(self.displacements), f)
    
        vol1 = vol2

tiff=Optical_FLow(r'.\heart_LV_ch01_100_res2.tiff')
tiff.optical_flow()