#include "functions.h"

void calculate_opticalflow(String *video_path, vector<Mat>*flow_fields, vector<Mat>* frames)
{
    Mat img_gray, flow;
    Ptr<cuda::FarnebackOpticalFlow> farn = cuda::FarnebackOpticalFlow::create();
    UMat  flowUmat, prevgray;
    for (int k = 0; k < frames->size(); k++) {


        Mat frame = frames->at(k);
        cv::cvtColor(frame, img_gray, COLOR_BGR2GRAY);
        GpuMat GpuImg0(img_gray);
        GpuMat GpuImg1(prevgray);
        GpuMat gflow(img_gray.size(), CV_32FC2);


        if (prevgray.empty() == false) {
            // Calculate optical flow
            farn->calc(GpuImg1, GpuImg0, gflow);
            // GpuMat to Mat
            gflow.download(flow);
            flow_fields->push_back(flow);
            visualizeflow(&flow, &frame);
        }
        img_gray.copyTo(prevgray);

        
    }
    destroyWindow("input");
    destroyWindow("flow");



}

                

