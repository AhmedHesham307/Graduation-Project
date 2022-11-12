#include "functions.h"

void visualizetrajectories(vector<vector<Mat>>* integrated_flowfields, Mat* trajectories_img, vector<Mat>* frames) {
    vector<Mat> xp;
    vector<Mat> yp;

    xp = integrated_flowfields->at(0);
    yp = integrated_flowfields->at(1);
    int rows = xp[0].rows;
    int cols = xp[0].cols;
    int t = xp.size();

    Point2f current, prev;

    for (int y = 0; y < rows; y += 5) {
        for (int x = 0; x < cols; x += 5) {
            prev = Point2f(x, y);
            for (int i = 0; i < t - 1; i++) {
                current = Point2f(xp[i + 1].at<float>(y, x), yp[i + 1].at<float>(y, x));
                if (prev != current) {
                    arrowedLine(*trajectories_img, prev, current, Scalar(abs(255 - i), i*2, i));
                }
                prev = current;

            }
        }
    } 

}
