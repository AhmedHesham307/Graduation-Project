#include "functions.h"

void calculate_opticalflow(GpuMat* flow_fields, GpuMat* frames, int size,GpuMat Mask)
{
    Ptr<cuda::FarnebackOpticalFlow> farn = cuda::FarnebackOpticalFlow::create();
    GpuMat  prevgray;
    GpuMat frame;
    GpuMat gflow(frame.size(), CV_32FC2);

    for (int k = 0; k < size; k++) {
        frame = frames[k];
        frames[k].release();
        cv::cuda::cvtColor(frame, frame, COLOR_BGR2GRAY);
        cv::cuda::bitwise_and(frame, Mask, frame);


        if (prevgray.empty() == false) {
            // Calculate optical flow
            farn->calc(prevgray, frame, gflow);

            flow_fields[k-1] = gflow;
            gflow.release();
        }
        frame.copyTo(prevgray);
        frame.release();


    }
    prevgray.release();
    frame.release();
    gflow.release();
}

