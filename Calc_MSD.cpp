#include "functions.h"


void slice(GpuMat* vec, GpuMat* vec_1, GpuMat* vec_2,int t,int size) {


    for (int k = t ; k < size; k++) {
        vec_1[k-t]=vec[k];
    }
    for (int k = 0; k < size - t; k++) {
        vec_2[k] = vec[k];
    }

}

void msd(GpuMat* xp_1, GpuMat* xp_2, GpuMat* yp_1, GpuMat* yp_2, GpuMat d, int Size) {
    GpuMat* temp=new GpuMat[Size];        

    for (int i = 0; i < Size; i++) {
        GpuMat x2, y2;
        GpuMat sub;
        cv::cuda::subtract(xp_1[i],xp_2[i],sub);
        cv::cuda::pow(sub, 2, x2);
        cv::cuda::subtract(yp_1[i], yp_2[i], sub);
        cv::cuda::pow(sub, 2, y2);
        cv::cuda::add(x2, y2, temp[i]);
        x2.release();
        y2.release();
        sub.release(); 
    }



    int rows, cols;
    rows = temp[0].rows;
    cols = temp[0].cols;


    
    for (int t = 0; t < Size; t++) {
        cv::cuda::add(temp[t], d, d);
        temp[t].release();
    }
    temp->release();
    cv::cuda::divide(d,  Size,d);
    
}





void Calc_MSD(GpuMat* xp, GpuMat* yp, GpuMat* MSD,int numOfFrames) {

    int rows, cols;
    rows = xp[0].rows;
    cols = xp[0].cols;
    int t = numOfFrames;
   /* Mat test;
    xp[255].download(test);
    cout << test << endl;*/


    for (int i = 0; i < t - 1; i++) {

        GpuMat* xp_1 = new GpuMat[t - i];
        GpuMat* xp_2 = new GpuMat[t - i];
        GpuMat* yp_1 = new GpuMat[t - i];
        GpuMat* yp_2 = new GpuMat[t - i];
        
        slice(xp,xp_1,xp_2, i, numOfFrames);
        slice(yp, yp_1, yp_2, i, numOfFrames);
        GpuMat d(rows, cols, CV_32FC1, Scalar(0));

        msd(xp_1,xp_2,yp_1,yp_2, d,t-i);
        MSD[i]=d;
        
    }

    cout << "MSD finished" << endl;
}   

