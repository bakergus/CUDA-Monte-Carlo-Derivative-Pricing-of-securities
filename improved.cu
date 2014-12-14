project
=======
#define MAX(a,b) ((a) > (b) ? a : b)
#define MIN(a,b) ((a) < (b) ? a : b)
__global__ void sum_improved_kernel(float *X, float Y, float *max, float *min, float maximum, float minimum)
{
__shared__ float partialSum[];

unsigned int t=threadIdx.x;
for(unsigned int stride=blockDim.x;stride>1;stride/=2)
{
__syncthreads();
if(t < stride)
partialSum[t]+=partialSum[t+stride];
max[t]= Max(X[t],X[t+stride]);
min[t]= Min(X[t],X[t+stride]);
}
Y=partialSum[blockDim.x];
maximum = max[blockDim.x];
minimum = min[blockDim.x];
}
