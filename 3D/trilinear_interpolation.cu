__global__
void trilinear_interpolation(int n,int axis,int * dim,float *x, float *y, float *z,float *intepolated_flow,float *vol)
{
    // x,y,z  1d size = (h*w*d) array positions where you want to find flow values
    //n total number of voxels in vol h*w*d
    //vol flow field carrying displacement in one direction  1d size = (h*w*d)
    //intepolated_flow storing output  1d size = (h*w*d)

  int i = blockIdx.x*blockDim.x + threadIdx.x;
  
  if (i < n) {
    int H=dim[0];
    int W=dim[1];
    int D=dim[2];
    float max =dim[axis];
    float next_pos=0.0;
    float X=x[i];
    float Y=y[i];
    float Z=z[i];
    if(axis==0){
      next_pos=X;
    }if(axis==1){
      next_pos=Y;
    }if(axis==2){
      next_pos=Z;
    }
    int X0=floor(X);
    int X1=ceil(X);
    int Y0=floor(Y);
    int Y1=ceil(Y);
    int Z0=floor(Z);
    int Z1=ceil(Z);
    float d_x=0;
    float d_y=0;
    float d_z=0;
    if (X0<0){
      //printf("%d \n", X0);
      X0=0;
    }
    if (X1<0){
      X1=0;
      ////printf("%d \n", X1);

    }
    if (X0>H-1){
      X0=H-1;
      //printf("%d \n", X0);
    }
    if (X1>H-1){
      X1=H-1;
      //printf("%d \n", X1);
    }
    if (Y0<0){
      Y0=0;
      //printf("%d \n", Y0);
    }
    if (Y1<0){
      Y1=0;
    //printf("%d \n", Y1);
    }
    if (Y0>W-1){
      Y0=W-1;
      //printf("%d \n", Y0);
    }
    if (Y1>W-1){
      Y1=W-1;
      //printf("%d \n", Y1);
    }
    if (Z0<0){
      Z0=0;
      //printf("%d \n", Z0);
    }
    if (Z1<0){
      Z1=0;
      //printf("%d \n", Z1);
    }
    if (Z0>D-1){
      Z0=D-1;
      //printf("%d \n", Z0);

    }
    if (Z1>D-1){
      Z1=D-1;
      //printf("%d \n", Z1);
    }

    if(X0!=X1){ 
        d_x=(X-X0)/(X1-X);
    }
    if(Y0!=Y1){
        d_y=(Y-Y0)/(Y1-Y);
    }
    if(Z0!=Z1){
        d_z=(Z-Z0)/(Z1-Z);
    }
    float c00,c01,c10,c11,c0,c1,c;
    c00=vol[X0*W*D+Y0*D+Z0]*(1-d_x)+vol[X1*W*D+Y0*D+Z0]*d_x;
    c01=vol[X0*W*D+Y0*D+Z1]*(1-d_x)+vol[X1*W*D+Y0*D+Z1]*d_x;
    c10=vol[X0*W*D+Y1*D+Z0]*(1-d_x)+vol[X1*W*D+Y1*D+Z0]*d_x;
    c11=vol[X0*W*D+Y1*D+Z1]*(1-d_x)+vol[X1*W*D+Y1*D+Z1]*d_x;


    c0=c00*(1-d_y)+c10*d_y;
    c1=c01*(1-d_y)+c11*d_y;

    c=c0*(1-d_z)+c1*d_z;
    c=c+next_pos;
    if(c >= max-1){
      c=0.0f;
    }
    if(c <= 0.0f){
      c=0.0f;
    }

    intepolated_flow[i]=c;

  }

}