#include "functions.h"

Mat Mask(Mat img) {
	//Mat Masked;
    namedWindow("mask", WINDOW_NORMAL);

    Mat binaryImg;
    //double thresh = cv::threshold(img, binaryImg, 0, 255, THRESH_OTSU);
    cv::threshold(img, binaryImg, 100, 255, THRESH_BINARY);
    //Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    //cv::dilate(binaryImg, binaryImg, kernel);

    //kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
    //cv::erode(binaryImg, binaryImg, kernel);

    //cv::bitwise_and(img, binaryImg, Masked);

    imshow("mask", binaryImg);




	return binaryImg;
}

Mat Mask_Nuc(Mat img) {
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
    imshow("nuc_mask", im_out);
    cv::cvtColor(img, img, COLOR_BGR2GRAY);

    cv::bitwise_and(img, im_out, Masked);

    cv::threshold(Masked, Masked, 110, 255, THRESH_BINARY);
    imshow("mask", Masked);

    return Masked;
}