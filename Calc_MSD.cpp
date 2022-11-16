#include "functions.h"


vector<vector<Mat>> slice(vector<Mat>vec,int t) {
    int size = vec.size();
    vector<Mat>vec1, vec2;

    vector<vector<Mat>> out;
    for (int k = t + 1; k < size; k++) {
        vec1.push_back(vec[k]);
    }
    for (int k = 0; k < size - t; k++) {
        vec2.push_back(vec[k]);
    }
    out.push_back(vec1);
    out.push_back(vec2);

    return out;
}

Mat msd(vector<vector<Mat>>xp_1_2, vector<vector<Mat>>yp_1_2) {
    vector<Mat>temp;
    for (int i = 0; i < xp_1_2[0].size(); i++) {
        Mat x2,y2;
        Mat sub;
        sub = xp_1_2[0][i]- xp_1_2[1][i];
        cv::pow(sub, 2, x2);
        sub = yp_1_2[0][i] - yp_1_2[1][i];
        cv::pow(sub, 2, y2);
        temp.push_back(x2+y2);     
    }

    int rows, cols;
    rows = temp[0].rows;
    cols = temp[0].cols;


    Mat d(rows, cols, CV_32FC1);

    for (int x = 0; x < rows; x++) {
        for (int y = 0; y < cols; y++) {
            float sum = 0.0;
            float avg = 0.0;
            for (int t = 0; t < temp.size(); t++) {
                sum = sum + temp[t].at<float>(Point(y, x));
            }
            avg = sum / temp.size();
            d.at<float>(Point(y, x)) = avg;
        }
    }



    return d;

}

vector<Mat> Calc_MSD(vector<vector<Mat>>* integrated_flowfields) {

    vector<Mat> xp;
    vector<Mat> yp;
    vector<Mat> MSD;

    xp = integrated_flowfields->at(0);
    yp = integrated_flowfields->at(1);



    int t = xp.size();

    for (int i = 0; i < t - 1; i++) {

        vector<vector<Mat>>xp_1_2;
        vector<vector<Mat>>yp_1_2;
        vector<Mat> d_x,d_y;
        xp_1_2 = slice(xp, i);
        yp_1_2 = slice(yp, i);
        Mat d;
        d = msd(xp_1_2, yp_1_2);
        cout << d.at<float>(Point(500, 200)) << endl;
        MSD.push_back(d);
        
    }
    return MSD;
}

