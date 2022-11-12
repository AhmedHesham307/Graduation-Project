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

void calculate_opticalflow(String *video_path, vector<Mat>* flow_fields, vector<Mat>* frames);
void visualizetrajectories(vector<vector<Mat>>*integrated_flowfields, Mat* trajectories_img, vector<Mat>* frames);
void visualizeflow(Mat* flow, Mat *img);
float BilinearInterpolation(float q11, float q12, float q21, float q22, int x1, int x2, int y1, int y2, float x, float y);
vector<vector<Mat>> integrateflowfield(vector<Mat>* flowfields);



#endif#include <iostream>
