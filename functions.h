#ifndef functions
#define functions

#include <iostream>
#include <fstream>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/cudabgsegm.hpp>
#include <opencv2/cudaobjdetect.hpp>
#include <opencv2/cudaoptflow.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/cudaarithm.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/cudacodec.hpp>
#include <omp.h>
#include <chrono>
#include <time.h>
#include<tuple>
#include<opencv2/core/cuda.hpp>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "opencv2/cudawarping.hpp "

using namespace std;
using namespace cv;
using namespace cv::cuda;


void calculate_opticalflow(GpuMat* flow_fields, GpuMat* frames, int size, GpuMat Mask);
void visualizetrajectories(vector<Mat>xp, vector<Mat>yp, Mat* trajectories_img);
void visualizeflow(GpuMat* flow, GpuMat* img);
void integrateflowfield(GpuMat* flowfields, GpuMat* xp, GpuMat* yp, int flowfields_len);
void Calc_MSD(GpuMat* xp, GpuMat* yp, GpuMat* MSD, int numOfFrames);
GpuMat Mask_Nuc(Mat img);

#endif#include <iostream>
