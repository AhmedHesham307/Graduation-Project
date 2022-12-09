#include "functions.h"
using namespace cv;
using namespace std;


int main(int argc, const char** argv)
{

    string path = "videos/dna.avi";
    VideoCapture cap(path);
    int  frame_Number = cap.get(CAP_PROP_FRAME_COUNT);
    cout << "number of frames= " << frame_Number << endl;
    cv::cuda::GpuMat* frames = new cv::cuda::GpuMat[frame_Number];
    int buffer = 0;
    Mat skipped;
    // Initialize eachof the frames
    for (int r = 0; r < buffer; r++) {

        cap.grab();

    }
    //cv::cuda::PtrStepSz<float> h_ptrs[256];
    cap.grab();
    cap.retrieve(skipped, CAP_OPENNI_GRAY_IMAGE);
    GpuMat Mask = Mask_Nuc(skipped);
    int rows = skipped.rows;
    int cols = skipped.cols;
    cout << "dimensions " << endl << "rows= " << rows << endl << "cols= " << cols << endl;
    for (int i = 0; i < frame_Number; ++i) {
        GpuMat GpuMat(skipped);
        frames[i] = GpuMat;
        GpuMat.release();
        cap.grab();
        cap.retrieve(skipped, CAP_OPENNI_GRAY_IMAGE);
    }
    cap.release();


    GpuMat* flow_fields = new cv::cuda::GpuMat[frame_Number-1];
    clock_t start, end;

    start = clock();
    calculate_opticalflow(flow_fields, frames, frame_Number,Mask);
    end = clock();

    cout << "the time to calculate the flow= " << ((double(end) - double(start)) / double(CLOCKS_PER_SEC)) << endl;
    cout << "AVG FPS = " << (frame_Number - buffer) / ((double(end) - double(start)) / double(CLOCKS_PER_SEC)) << endl;

    frames->release();

    GpuMat* xp = new GpuMat[frame_Number];
    GpuMat* yp = new GpuMat[frame_Number];
    start = clock();
    integrateflowfield(flow_fields, xp, yp, frame_Number-1);
    end = clock();
    cout << "the time to integrate the flow= " << ((double(end) - double(start)) / double(CLOCKS_PER_SEC)) << endl;

    GpuMat* MSD = new GpuMat[frame_Number-1];
    start = clock();
    Calc_MSD(xp, yp, MSD, frame_Number);
    end = clock();
    cout << "the time for MSD calc= " << ((double(end) - double(start)) / double(CLOCKS_PER_SEC)) << endl;



    vector<Mat> YP, XP;

    for (int i = 0; i < frame_Number-1; ++i) {
        Mat tempx;
        xp[i].download(tempx);
        XP.push_back(tempx);
        xp[i].release();
        Mat tempy;
        yp[i].download(tempy);
        YP.push_back(tempy);
        yp[i].release();

    }




    xp->release();
    yp->release();

    Mat trajectories(rows * 10, cols * 10, CV_8UC3, Scalar(255, 255, 255));

    visualizetrajectories(XP,YP, &trajectories);


    namedWindow("trajectories", WINDOW_NORMAL);
    imshow("trajectories", trajectories);
    waitKey(0);
}

