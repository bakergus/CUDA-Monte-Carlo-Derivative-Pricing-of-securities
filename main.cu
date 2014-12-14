/*------------------------------------------------------
         Template taken from vecadd
--------------------------------------------------------*/

#include <stdio.h>
#include <sys/time.h>
#include "kernel.cu"

#define M 20 //number of assets
#define N 200 //number of simus
#define blocksize 32


static void launch_simple_kernel(float *X, float Y, float *max, float *min, float maximum, float minimum)
 {
    dim3 dimGrid(blocksize,1,1);
    dim3 dimBlock(M,(N-1)/blocksize+1,1);
    sum_simple_kernel<<<dimGrid,dimBlock>>>(float *X, float Y, float *max, float *min, float maximum, float minimum);
}

static void launch_improved_kernel(float *X, float Y, float *max, float *min, float maximum, float minimum,int Inputsize)
 {
    dim3 dimGrid(blocksize,1,1);
    dim3 dimBlock(M,(N-1)/blocksize+1,1);
    sum_improved_kernel<<<dimGrid,dimBlock>>>(float *X, float Y, float *max, float *min, float maximum, float minimum);
}



// Declare error and timing utilities =========================================

#define FATAL(msg, ...) \
    do {\
        fprintf(stderr, "[%s:%d] "msg"\n\n", __FILE__, __LINE__, ##__VA_ARGS__);\
        exit(-1);\
    } while(0)

typedef struct {
    struct timeval startTime;
    struct timeval endTime;
} Timer;
void startTime(Timer* timer);
void stopTime(Timer* timer);
float elapsedTime(Timer timer);

// Main function ==============================================================
int main(float *X,float Y, int InputSize) {

    Timer timer;
    cudaError_t cuda_ret;

    // Initialize GPU/CUDA ----------------------------------------------------

    printf("\nInitializing GPU/CUDA..."); fflush(stdout);
    startTime(&timer);
    cudaDeviceSynchronize();
    stopTime(&timer); printf("%f s\n", elapsedTime(timer));

    // Input parameters and host variables -------------------------

    printf("Setting up the problem..."); fflush(stdout);
    startTime(&timer);
     
     unsigned int nSimu, nSteps;
    nSimu = 200;// 200 different predictions for each stock
    nSteps = 100;//100 time points (,time steps)

     float* X;
     int Inputsize=sizeof(float)* N * M;
     int size=sizeof(float);
    cuda_ret = cudaMalloc((void**) &X, sizeof(float)*nSimu*nAssets);
    if(cuda_ret != cudaSuccess) FATAL("Unable to allocate device memory");

    stopTime(&timer); printf("%f s\n", elapsedTime(timer));
    __syncthreads();

    startTime(&timer);
    launch_simple_kernel(float X, float Y, float max, float min, float maximum, float minimum);
    stopTime(&timer); printf("%f s\n", elapsedTime(timer));


    startTime(&timer);
    launch_improved_kernel(float X, float Y, float max, float min, float maximum, float minimum);
    stopTime(&timer); printf("%f s\n", elapsedTime(timer));

    

    stopTime(&timer);
    int InputSize = n *sizeof(float);
    cudaMalloc((void**)&X,Inputsize);
    cudaMemcpy(X,price,Inputsize, cudaMemcpyHostToDevice);

    cudaMalloc((void**)&Y,size);
    
    cudaMemcpy(Y,Y,size, cudaMemcpyDeviceToHost);
    cudaFree(X);
    cudaFree(Y);

    stopTime(&timer); printf("%f s\n", elapsedTime(timer));
}
