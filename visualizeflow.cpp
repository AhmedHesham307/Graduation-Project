#include "functions.h"
void visualizeflow(GpuMat*flow, GpuMat*img) {
    GpuMat colors;

    GpuMat flow_parts[2];
    cv::cuda::split(*flow, flow_parts);

    // Convert the algorithm's output into Polar coordinates
    GpuMat magnitude, angle;
    cv::cuda::cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);
    cv::cuda::multiply(angle,((1.f / 360.f) * (180.f / 255.f)),angle);
    GpuMat _hsv[3], hsv, hsv8, bgr;
    _hsv[0] = angle;
    _hsv[1] = GpuMat(angle.size(), 1.0,CV_32F);
    _hsv[2] = GpuMat(angle.size(),1.0, CV_32F);;
    cv::cuda::merge(_hsv, 3, hsv);
    hsv.convertTo(hsv8, CV_8U, 255.0);

    // Display the results
    cv::cuda::cvtColor(hsv8, colors, COLOR_HSV2BGR);


    Vec3b color;
    GpuMat whiteimage(img->rows, img->cols, CV_8UC3, Scalar(255, 255, 255));
    Point2f flowatxy;
    for (int y = 0; y < img->rows; y += 4) {
        for (int x = 0; x < img->cols; x += 8) {
            flowatxy = flow->at<Point2f>(y, x)*10;
            color = colors.at<Vec3b>(Point2f(x, y));
            if(!(flowatxy.x==0 && flowatxy.y==0))
                arrowedLine(whiteimage, Point(x, y), Point((x + flowatxy.x), (y + flowatxy.y)), Scalar((float)color[0], (float)color[1], (float)color[2]));

        }
    }

    namedWindow("input", WINDOW_AUTOSIZE);
    imshow("input", *img);
    namedWindow("flow", WINDOW_AUTOSIZE);
    imshow("flow", whiteimage);
    waitKey(1);

}