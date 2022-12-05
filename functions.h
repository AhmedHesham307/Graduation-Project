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
#include <omp.h>
#include <chrono>
#include <time.h>
#include<tuple>



using namespace std;
using namespace cv;
using namespace cv::cuda;


void calculate_opticalflow(vector<Mat>* flow_fields, VideoCapture cap,int num_frames);
void visualizetrajectories(vector<vector<Mat>>*integrated_flowfields, Mat* trajectories_img);
void visualizeflow(Mat* flow, Mat *img);
vector<vector<Mat>> integrateflowfield(vector<Mat>* flowfields);
vector<Mat> Calc_MSD(vector<vector<Mat>>* integrated_flowfields);
void visualizeMSD(vector<Mat> MSD);
Mat Mask(Mat img);
Mat Mask_Nuc(Mat img);
#endif#include <iostream>
