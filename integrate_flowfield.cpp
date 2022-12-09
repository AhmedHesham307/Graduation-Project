#include "functions.h"

void integrateflowfield(GpuMat* flowfields, GpuMat*xp, GpuMat*yp,int flowfields_len){

	GpuMat flow_parts[2];
	GpuMat* u = new GpuMat[flowfields_len];
	GpuMat* v = new GpuMat[flowfields_len];

	for (int i = 0; i < flowfields_len; i++) {
		cv::cuda::split(flowfields[i], flow_parts);
		u[i]=flow_parts[0];
		v[i] = flow_parts[1];
		flowfields[i].release();



	}
	flowfields->release();
	flow_parts->release();


	int rows = u[0].rows;
	int cols = u[0].cols;
	//cout << rows << cols << endl;
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







	/*for (int k = 0; k <256; k++) {
		xp[k] = GpuMat (rows, cols, CV_32FC1, 0.0);
		yp[k] = GpuMat(rows, cols, CV_32FC1, 0.0);;
	}*/
	xp->setTo(0);
	yp->setTo(0);
	xp[0].upload(x);
	yp[0].upload(y);



	for (int t = 0; t < flowfields_len; t++) {
		GpuMat temp_x; 
		GpuMat temp_y; 
		

		cv::cuda::remap(u[t], temp_x,xp[t], yp[t], INTER_LINEAR);
		cv::cuda::add(xp[t], temp_x, xp[t + 1]);
		cv::cuda::remap(v[t], temp_y, xp[t], yp[t], INTER_LINEAR);
		cv::cuda::add(yp[t], temp_y, yp[t + 1]);


	}


	for (int i = 0; i < flowfields_len; i++) {

		u[i].release();
		v[i].release();



	}
	u->release();
	v->release();


}
	