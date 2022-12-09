#include "functions.h"

void visualizetrajectories(vector<Mat>xp, vector<Mat>yp, Mat* trajectories_img){


    int rows = xp[0].rows;
    int cols = xp[0].cols;
    int t = xp.size();

    Point2f current, prev;

    for (int y = 0; y < rows; y += 1) {
        for (int x = 0; x < cols; x += 1) {
            prev = Point2f(x, y)*10.0;
            for (int i = 0; i < t - 1; i++) {
                current = Point2f(xp[i + 1].at<float>(y, x), yp[i + 1].at<float>(y, x))*10.0;
                if (Point(prev.x,prev.y)!=Point(current.x,current.y)) {
                    arrowedLine(*trajectories_img, prev, current, Scalar(220-i, 100+0.75*i ,  0.9*i));
                }
                prev = current;

            }
        }
    } 


    for (int x = 3300; x < 5850; x += 1) {
        for (int y = x; y < x + 10; y++) {
            arrowedLine(*trajectories_img, Point(y, 4500), Point(y, 4300), Scalar(220- 0.1*(x - 3300), 100 + 0.075 * (x - 3300), 0.09*(x - 3300)));

        }
    }
}
