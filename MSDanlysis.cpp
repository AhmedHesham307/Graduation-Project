#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\objdetect\objdetect.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/opencv.hpp"
#include <vector>
#include <stdio.h>
#include <Windows.h>
#include <iostream>
#include <time.h>
//#include"Conversion_from_flowfields _to _trajectories.cpp"

using namespace cv;
using namespace std;

vector<float> msdCalc(vector<Point2f> points1) {
    int point_count = points1.size();
    vector<Point2f> points2;
    vector<Point2f> points2_s;
    vector<float> MSD(point_count);
    for (int i = 0; i < point_count; i++) {

        for (int k = 0; k < point_count - i; k++) {
            points2_s.push_back(points1[k]);
        }
        cout << " first vector" << endl;
        for (int i = 0; i < points2_s.size(); i++) {
            cout << points2_s[i];
        }
        cout << endl;
        int h = 0;

        for (int k = i; k < point_count; k++) {
            points2.push_back(points1[k]);
        }
        cout << " second vector" << endl;
        for (int u = 0; u < points2.size(); u++) {
            cout << points2[u];
        }
        cout << "squared difference" << endl;
        vector<Point2f> difference(points2_s.size());
        for (int j = 0; j < points2.size(); j++) {
            difference[j].x = (points2[j].x - points2_s[j].x);
            difference[j].y = (points2[j].y - points2_s[j].y);
        }
        for (int j = 0; j < points2.size(); j++) {
            difference[j].x = pow(difference[j].x, 2);
            difference[j].y = pow(difference[j].y, 2);
        }
        for (int j = 0; j < points2.size(); j++) {
            cout << difference[j].x <<" ";
            cout << difference[j].y<< " ";
        }
        cout << endl;


        cv::Mat mean_;
        cv::reduce(difference, mean_, 01, 1);
        // convert from Mat to Point - there may be even a simpler conversion,
        // but I do not know about it.
        Point2f mean1(mean_.at<float>(0, 0), mean_.at<float>(0, 1));
        cout << mean1 << endl;
        MSD[i] = (mean1.x + mean1.y);
        points2_s.clear();
        points2.clear();
        difference.clear();

    }
    return MSD;
}
int main() {

    float point_count = 6;
    vector<Point2f> points1(point_count);
    vector<Point2f> points2;

    points1 = { Point2f(0.4f,0.6f) , Point2f(0.6f,0.9f) ,Point2f(0.7f,1.1f) ,Point2f(1.3f,1.4f) ,Point2f(1.5f,1.6f), Point2f(1.6f,1.9f) };
    points2 = { Point2f(0.5f,0.7f) , Point2f(0.7f,0.8f) ,Point2f(0.9f,1.2f) ,Point2f(1.1f,1.4f) };
    vector<float> time_steps(point_count);
    vector <float> msd(point_count, 0);
    vector <float> msd2;
    time_steps = { 0.1,0.2,0.3,0.4,0.5,0.6 };
    msd = msdCalc(points1);
    msd2 = msdCalc(points2);
    cout << "-----------------" << endl;
    for (int i = 0; i < msd.size(); i++) {
        cout << msd[i] << endl;
    }

    cout << " second msd " << endl;
    for (int i = 0; i < msd2.size(); i++) {
        cout << msd2[i] << endl;
    }

    return 0;
}