#include "functions.h"
using namespace cv;
using namespace std;



int main(int argc, const char** argv)
{

    string path= "videos/dna.avi";
    vector<Mat> flowfields;
    VideoCapture cap(path);
    clock_t start;
    clock_t end;
    double frame_Number = cap.get(CAP_PROP_FRAME_COUNT);
    Mat img;
    //bool Is = cap.grab();
    //cap.retrieve(img, CAP_OPENNI_GRAY_IMAGE);

    cout << frame_Number << endl;


    /*Mat skipped;
    int buffer = 444;
    for (int r = 0; r < buffer; r++) {
        bool Is = cap.grab();
        cap.retrieve(skipped, CAP_OPENNI_GRAY_IMAGE);

    }*/
    //cout << "num of frames = " << frame_Number << endl;
    //cout << "frame size:" << endl << "rows " << img.rows << endl << "cols " << img.cols << endl;

    start = clock();
    calculate_opticalflow(&flowfields,cap,frame_Number);
    end = clock();
    cout << "AVG FPS = " << (frame_Number   )/ ((double(end) - double(start)) / double(CLOCKS_PER_SEC)) << endl;
    cout << "Time for flow calculations = " << ((double(end) - double(start)) / double(CLOCKS_PER_SEC)) << endl;



    Mat trajectories(flowfields[0].rows*10, flowfields[0].cols*10, CV_8UC3, Scalar(255, 255, 255));
    start = clock();
    vector<vector<Mat>> integratedflowfields;
    integratedflowfields=integrateflowfield(&flowfields);

    end = clock();
    cout << "time for interpolation= " << (double(end) - double(start)) / double(CLOCKS_PER_SEC)<< endl;
    start = clock();
    visualizetrajectories(&integratedflowfields, &trajectories);
    end = clock();
    cout << "time for trajectories visualization= " << (double(end) - double(start)) / double(CLOCKS_PER_SEC) << endl;
    start = clock();

    vector<Mat> MSD;
    MSD = Calc_MSD(&integratedflowfields);
    end = clock();
    cout << "time for MSD calculations= " << (double(end) - double(start)) / double(CLOCKS_PER_SEC) << endl;
    namedWindow("trajectories", WINDOW_NORMAL);
    imshow("trajectories", trajectories);
    waitKey(0);
    visualizeMSD(MSD);


}