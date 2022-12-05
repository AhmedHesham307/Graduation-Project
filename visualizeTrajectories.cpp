#include "functions.h"

void visualizetrajectories(vector<vector<Mat>>* integrated_flowfields, Mat* trajectories_img) {
    vector<Mat> xp;
    vector<Mat> yp;

    xp = integrated_flowfields->at(0);
    yp = integrated_flowfields->at(1);
    int rows = xp[0].rows;
    int cols = xp[0].cols;
    int t = xp.size();
    cout << rows << " " << cols << endl;
    Point2f current, prev;

    for (int y = 0; y < rows; y += 1) {
        for (int x = 0; x < cols; x += 1) {
            prev = Point2f(x, y)*10.0;
            for (int i = 0; i < t - 1; i++) {
                current = Point2f(xp[i + 1].at<float>(y, x), yp[i + 1].at<float>(y, x))*10.0;
                if (Point(prev.x,prev.y)!=Point(current.x,current.y)) {
                    arrowedLine(*trajectories_img, prev, current, Scalar(220-2*i, 100+0.9*i ,  0.9*i));
                }
                prev = current;

            }
        }
    } 


    for (int x = 1000; x < 2500; x += 1) {
        for (int y = x; y < x + 10; y++) {
            arrowedLine(*trajectories_img, Point(y, rows*10), Point(y, rows*10-100), Scalar(220- 0.2*(x - 1000), 100 + 0.09 * (x - 1000), 0.09*(x - 1000)));

        }
    }
}
