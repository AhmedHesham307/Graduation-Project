#include "functions.h"
using namespace cv;
using namespace std;

void color_map(cv::Mat& input /*CV_32FC1*/, cv::Mat& dest, int color_map);

int main(int argc, const char** argv)
{
    cv::setBreakOnError(true);

    string path= "2d.mp4";
    vector<Mat> frames, flowfields;
    VideoCapture cap(path);
    clock_t start;
    clock_t end;
    //cout << BilinearInterpolation(12,0,-4, 8, 0, 4, 1, 3, 1, 2) << endl;   

    for (int i = 0; i < 700;i++) {

        Mat frame;
        // Capture frame-by-frame
        cap >> frame;
        if (frame.empty())
            break;
        if(i>650)
            frames.push_back(frame);
        
    }
    cout << "num of frames = " << frames.size() << endl;
    cout << "frame size:" << endl << "rows " << frames[0].rows << endl << "cols " << frames[0].cols << endl;

    start = clock();
    calculate_opticalflow(&path,&flowfields,&frames);
    end = clock();
    cout << "AVG FPS = " <<  frames.size()/ ((double(end) - double(start)) / double(CLOCKS_PER_SEC)) << endl;


    
    Mat trajectories(flowfields[0].rows, flowfields[0].cols, CV_8UC3, Scalar(255, 255, 255));
    start = clock();
    vector<vector<Mat>> integratedflowfields;
    integratedflowfields=integrateflowfield(&flowfields);

    end = clock();
    cout << "time for interpolation= " << (double(end) - double(start)) / double(CLOCKS_PER_SEC)<< endl;
    visualizetrajectories(&integratedflowfields, &trajectories,&frames);

    vector<Mat> MSD;
    MSD = Calc_MSD(&integratedflowfields);

    namedWindow("trajectories",0);
    //applyColorMap(trajectories, trajectories, COLORMAP_VIRIDIS);
    //color_map(trajectories, trajectories, COLORMAP_DEEPGREEN);
    imshow("trajectories", trajectories);
    waitKey(0);

}

void color_map(cv::Mat& input /*CV_32FC1*/, cv::Mat& dest, int color_map) {

    int num_bar_w = 30;
    int color_bar_w = 10;
    int vline = 10;

    cv::Mat win_mat(cv::Size(input.cols + num_bar_w + num_bar_w + vline, input.rows), CV_8UC3, cv::Scalar(255, 255, 255));

    //Input image to
    double Min, Max;
    cv::minMaxLoc(input, &Min, &Max);
    int max_int = ceil(Max);

    std::cout << " Min " << Min << " Max " << Max << std::endl;

    input.convertTo(input, CV_8UC3, 255.0 / (Max - Min), -255.0 * Min / (Max - Min));
    input.convertTo(input, CV_8UC3);

    cv::Mat M;
    cv::applyColorMap(input, M, color_map);

    M.copyTo(win_mat(cv::Rect(0, 0, input.cols, input.rows)));
    //Scale
    cv::Mat num_window(cv::Size(num_bar_w, input.rows), CV_8UC3, cv::Scalar(255, 255, 255));
    for (int i = 0; i <= max_int; i++) {
        int j = i * input.rows / max_int;
        cv::putText(num_window, std::to_string(i), cv::Point(5, num_window.rows - j - 5), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 0), 1, 2, false);
    }

    //color bar
    cv::Mat color_bar(cv::Size(color_bar_w, input.rows), CV_8UC3, cv::Scalar(255, 255, 255));
    cv::Mat cb;
    for (int i = 0; i < color_bar.rows; i++) {
        for (int j = 0; j < color_bar_w; j++) {
            int v = 255 - 255 * i / color_bar.rows;
            color_bar.at<cv::Vec3b>(i, j) = cv::Vec3b(v, v, v);
        }
    }

    color_bar.convertTo(color_bar, CV_8UC3);
    cv::applyColorMap(color_bar, cb, color_map);
    num_window.copyTo(win_mat(cv::Rect(input.cols + vline + color_bar_w, 0, num_bar_w, input.rows)));
    cb.copyTo(win_mat(cv::Rect(input.cols + vline, 0, color_bar_w, input.rows)));
    dest = win_mat.clone();
}
