#include "functions.h"

GpuMat Mask_Nuc(Mat img) {
    Mat im_th;
    cv::threshold(img, im_th, 120, 255, THRESH_BINARY);

    // Floodfill from point (0, 0)
    Mat im_floodfill = im_th.clone();
    floodFill(im_floodfill, cv::Point(0, 0), Scalar(255));

    // Invert floodfilled image
    Mat im_floodfill_inv;
    cv::bitwise_not(im_floodfill, im_floodfill_inv);

    // Combine the two images to get the foreground.

    Mat im_out = (im_th | im_floodfill_inv);
    Mat Masked;
    //cv::bitwise_and(img, im_out, Masked);
    cv::cvtColor(im_out, im_out, COLOR_BGR2GRAY);
    cv::threshold(im_out, im_out, 0, 255, THRESH_OTSU);
    cv::cvtColor(img, img, COLOR_BGR2GRAY);

    cv::bitwise_and(img, im_out, Masked);

    cv::threshold(Masked, Masked, 110, 255, THRESH_BINARY);
    GpuMat mask(Masked);
    return mask;
}