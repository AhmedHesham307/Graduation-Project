#include "functions.h"

void visualizeMSD(vector<Mat> MSD) {

	vector<Point>points = { Point(196,196),Point(194,194) ,Point(195,195) ,Point(193,193) };
	int Size = MSD.size();
	int rows = 1500;
	Mat white_img(rows, Size*5, CV_8UC3, Scalar(255, 255, 255));

	

	for (int i = 0; i < rows; i += 10)
		cv::line(white_img, Point(0, i), Point(Size*5, i), cv::Scalar(220, 220, 220));

	for (int i = 0; i < Size*5; i +=5 )
		cv::line(white_img, Point(i, 0), Point(i, rows), cv::Scalar(220, 220, 220));


	for (int p = 0; p < points.size(); p++) {

		Point prev = Point(0, rows - MSD[0].at<float>(points[p]) );
		Point current;
		for (int i = 1; i < Size - 1; i++) {
			current = Point(i*5, rows - MSD[i].at<float>(points[p]) );
			line(white_img, prev, current, Scalar(50*p, 50*p, 255-p*50));
			prev = current;

		}
	}

	namedWindow("MSD", WINDOW_NORMAL);


	imshow("MSD", white_img);
	waitKey(0);
}