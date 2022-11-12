#include "functions.h"
#include <armadillo>



vector<vector<Mat>> integrateflowfield(vector<Mat>* flowfields) {

	Mat flow_parts[2];
	vector<Mat> u, v;
	for (int i = 0; i < flowfields->size(); i++) {
		split(flowfields->at(i), flow_parts);
		u.push_back(flow_parts[0]);
		v.push_back(flow_parts[1]);
	}


	int rows = u[0].rows;
	int cols = u[0].cols;

	Mat x(rows, cols, CV_32FC1);
	Mat y(rows, cols, CV_32FC1);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			x.row(i).col(j) = j;
		}
	}

	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {
			y.row(j).col(i) = j;
		}

	}




	vector<Mat> xp, yp;
	for (int k = 0; k < u.size() + 1; k++) {

		Mat zerosmatx(rows, cols, CV_32FC1, 0.0);
		Mat zerosmaty(rows, cols, CV_32FC1, 0.0);
		xp.push_back(zerosmatx);
		yp.push_back(zerosmaty);
	}
	xp[0] = x;
	yp[0] = y;



	for (int t = 0; t < u.size() ; t++) {
		Mat temp_x, temp_y;

		remap(u[t], temp_x,xp[t], yp[t], INTER_LINEAR);
		xp[t + 1]= xp[t]+ temp_x;
		remap(v[t], temp_y, xp[t], yp[t], INTER_LINEAR);
		//cout << v[t].at<float>(Point(500, 200)) << endl;

		//cout << temp_y.at<float>(Point(500, 200)) << endl;
		yp[t + 1] = yp[t] + temp_y;
		//cout << yp[t].at<float>(Point(500, 200)) << endl;
		//cout << yp[t + 1].at<float>(Point(500, 200)) << endl;


	}
	//cout << yp[10].at<float>(Point(500, 200)) << endl;
	vector<vector<Mat>> integratedflowfields;
	integratedflowfields.push_back(xp);
	integratedflowfields.push_back(yp);
	
	return integratedflowfields;

}
	