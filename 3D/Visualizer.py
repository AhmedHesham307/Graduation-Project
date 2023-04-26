import numpy as np
from pickle import load
import os
from tqdm import tqdm
import imageio
import pickle as pk
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time
import pyvista as pv
import gzip
import scipy
import cv2

class visualizer:

    def __init__(self) -> None:

        # Global Variables 
        self.plotter = pv.Plotter()
        self.shape = (74, 82, 83) #(202, 237, 242)
    
    
    def flow_data_vis(self):
        self.__read_data()
        self.__read_flow()
        print(np.shape(self.Fields))
        print(np.shape(self.volumes))
        data_pos = np.ravel(self.volumes[2])
        i = 1
        point_cloud = np.where(self.volumes[int(np.floor(i+1))] != 0)
        # swap axes because the plotter needs a shape of (n , 3)
        point_cloud = np.swapaxes(point_cloud , 0,1)
        self.plotter.add_points(point_cloud, render_points_as_spheres=True,
                            point_size=2.0)
        
        positions = np.where(np.reshape(self.Fields[i][0] , self.shape) != 0 )
            
        # this has the shape of flow fields but with true or false indicating whether or not there is a flow there
        boolean_positions = [data_pos != 0 ]

        """
        directions contain the flow values at the locations where boolean_positions = True
        its shape is again (3 , number of locations where flow != 0)
        """
        directions = [np.extract(boolean_positions , np.round(self.Fields[i][0] , decimals = 2)),
                    np.extract(boolean_positions , np.round(self.Fields[i][1] , decimals = 2)),
                    np.extract(boolean_positions , np.round(self.Fields[i][2] , decimals = 2))]
        
        self.plotter.add_arrows(np.argwhere(self.volumes[i+1] !=0), np.swapaxes(directions,0,1), mag = 1, cmap = 'viridis')

        self.plotter.show()


    def Data_visualization(self , n_frames = 300):
        flag = True
        # read data
        self.__read_data()
        # open a mp4 file to write on
        self.plotter.open_movie("./output.mp4")

        # loop over the entire data
        for i in np.linspace(0, self.volumes.shape[0], n_frames + 1)[:n_frames]:
            i = int(np.floor(i))
            # reshape the data volumes to a point cloud with shape (3,number of points in the grid)
            point_cloud = np.where(self.volumes[int(np.floor(i))] != 0)
            # swap axes because the plotter needs a shape of (n , 3)
            point_cloud = np.swapaxes(point_cloud , 0,1)
            
            # # first iteration we need to add_points to the plotter
            # self.plotter.clear()
            # # add our point cloud to the plotter
            self.plotter.add_points(point_cloud, render_points_as_spheres=True,
                        point_size=2.0)
            
            # write the frame on the mp4 
            self.plotter.write_frame()
            self.plotter.clear_actors()

        # close the plotter
        self.plotter.close()

    def __read_data(self):
        # Read data
        # self.volumes = imageio.imread(r"F:\materials\Fourth year\GP\Code\Data\heart_LV_chr01_02_100_comp.tiff")
        self.volumes = imageio.imread(r".\heart_LV_ch01_100_res2.tiff")


    def Flow_visualization(self , n_frames = 300):
        self.__read_flow()
        self.__read_data()

        actors = []
        for i in tqdm(range(np.shape(self.Fields)[0])):
            data_pos = np.ravel(self.volumes[i+1])
            print(np.shape(self.Fields) , np.shape(data_pos))
            """
            positions holds the location of where the flow value doesn't equal 0 in the mesh  
            the shape will be (3 , number of locations where flow != 0)
            """
            # flow = np.reshape(self.Fields[i][0] , (74, 82, 83))
            
            # this has the shape of flow fields but with true or false indicating whether or not there is a flow there
            boolean_positions = [data_pos != 0 ]

            """
            directions contain the flow values at the locations where boolean_positions = True
            its shape is again (3 , number of locations where flow != 0)
            """
            directions = [np.extract(boolean_positions , np.round(self.Fields[i][0] , decimals = 1)),
                        np.extract(boolean_positions , np.round(self.Fields[i][1] , decimals = 1)),
                        np.extract(boolean_positions , np.round(self.Fields[i][2] , decimals = 1))]
            print(f"the flow shape = {np.shape(directions)}")
            
            # plot arrows at the positions with these directions
            actors.append(self.plotter.add_arrows(np.argwhere(self.volumes[i+1] !=0), np.swapaxes(directions,0,1),
                                                   mag = 2, cmap = 'viridis')) #, log_scale = True))
            actors[i].visibility = False

        self.plotter.open_movie("./output.mp4")
    
        for i in np.linspace(0, np.shape(self.Fields)[0] , n_frames + 1)[:n_frames]:
            i = int(np.floor(i))
            actors[i].visibility = True
            self.plotter.write_frame()
            actors[i].visibility = False

        # self.plotter.close()

    def __read_flow(self):
        # Read Flow Fields
        flow_folder_path=r".\fields_out"
        # create a list of the file names inside the folder
        files = os.listdir(flow_folder_path)

        self.Fields=[]
        # loop on the files in the folder
        for index,file in tqdm(enumerate(files)) :
            # join the file path with the folder path
            file_path = os.path.join(flow_folder_path,file)
            # create an instance of a readable file
            file = gzip.GzipFile(file_path , "r")  
            # read the data and append it in the global variable with shape (3 , height * width * depth)         
            data =  np.load(file , allow_pickle = True)

            # print(len(np.unique(data)))
            
            self.Fields.append([np.ravel(data[0]) , np.ravel(data[1]) , np.ravel(data[2])])

    def Trajectory_visualization(self):
        self.__read_trajectories()
        print(f"positions shape = {np.shape(self.Positions)}")
        self.Positions = np.swapaxes(self.Positions , 1,2)
        colors = ['red' , 'blue' , 'green']
        actors = []
        # self.plotter.open_movie("./output.mp4")
        for i in range(np.shape(self.Positions)[0]):
            if i ==0 :
                continue
            lines = []
            for j in range(np.shape(self.Positions)[1]):
                sub = self.Positions[i][j] - self.Positions[i-1][j] 

                if np.any(sub) and max(sub) <5 and min(sub) > -5 and not np.any(np.isnan(sub)):
                    lines.append(self.Positions[i][j])
                    lines.append(self.Positions[i-1][j])
                    # print(self.Positions[i-1][j])
                    # print(self.Positions[i][j])
            
            lines = np.array(lines)
            print(f"lines created = {lines.shape}")

            actors.append(self.plotter.add_lines(lines , color= [1.0/(i*1.5) , 1.0/(i*1.2) , 1.0/(i*1.1)], width=1))
        # self.plotter.show()
            actors[i-1].visibility = False
            
        self.plotter.open_movie("./output.mp4")

        n_frames = 300
        for i in np.linspace(0, np.shape(self.Positions)[0]-1 , n_frames + 1)[:n_frames]:
            i = int(np.floor(i))
            actors[i].visibility = True
            self.plotter.write_frame()
            # actors[i].visibility = False
        self.plotter.camera_position = 'xy'
        self.plotter.close()
        

    def __read_trajectories(self):
        folder_path=r".\positions_out"
        files = os.listdir(folder_path)

        self.Positions=[]
        for index,file in tqdm(enumerate(files)) :
            file_path=os.path.join(folder_path,file)

            file = gzip.GzipFile(file_path , "r")  
            # read the data and append it in the global variable with shape (3 , height * width * depth)         
            position = np.load(file , allow_pickle = True)
            
            self.Positions.append(position)


vis = visualizer()
vis.Trajectory_visualization()