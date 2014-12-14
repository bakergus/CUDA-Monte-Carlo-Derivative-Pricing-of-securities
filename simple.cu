project
=======
#define MAX(a,b) ((a) > (b) ? a : b)
#define MIN(a,b) ((a) < (b) ? a : b)

__global__ void sum_simple_kernel(float *X, float Y, float *max, float *min, float maximum, float minimum)
{
__shared__ float partialSum[];
unsigned int t=threadIdx.x;
for(unsigned int stride=1;stride<blockDim.x;stride*=2)
{
__syncthreads();
if(t % (2*stride) ==0)
partialSum[t]+=partialSum[t+stride];
max[t]= Max(X[t],X[t+stride]);
min[t]= Min(X[t],X[t+stride]);
}
Y=partialSum[blockDim.x];
maximum = max[blockDim.x];
minimum = min[blockDim.x];
}

