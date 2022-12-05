#include "functions.h"

void calculate_opticalflow(vector<Mat>*flow_fields, VideoCapture cap,int num_frames)
{
    Mat frame;

    Mat Masked, flow, prevgray;
    bool Is = cap.grab();
    cap.retrieve(frame, CAP_OPENNI_GRAY_IMAGE);


    Mat Mask_nuc = Mask_Nuc(frame);
    Ptr<cuda::FarnebackOpticalFlow> farn = cuda::FarnebackOpticalFlow::create();
    GpuMat GpuImg0;
    GpuMat GpuImg1;
    GpuMat gflow;
    while (Is) {





        cv::cvtColor(frame, frame, COLOR_BGR2GRAY);

        //img_gray = Mask(frame);
        cv::bitwise_and(frame, Mask_nuc, Masked);




        


        GpuImg0.upload(Masked);
        GpuImg1.upload( prevgray);
        if (prevgray.empty() == false) {
            // Calculate optical flow
            farn->calc(GpuImg1, GpuImg0, gflow);
            // GpuMat to Mat
            gflow.download(flow);
            flow_fields->push_back(flow);
            visualizeflow(&flow, &frame);
        }
        Masked.copyTo(prevgray);
        Is = cap.grab();
        cap.retrieve(frame, CAP_OPENNI_GRAY_IMAGE);

    }
    destroyWindow("input");
    destroyWindow("flow");
    destroyWindow("mask");
    destroyWindow("nuc_mask");


    


}

                

