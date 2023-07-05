#include <stdio.h>
#include <iostream>
#include <cuda_runtime.h>
// #include <helper_cuda.h>

__global__ void vec_gpu_add(const float* a,const float* b,float* c,int size)
{
    int i = threadIdx.x +  blockDim.x * blockIdx.x;
    if(i < size)
    {
        c[i] = a[i] + b[i];
    }
};

int main()
{
    cudaSetDevice(0);
    int size_test = 5000;
    float *a = (float*)malloc(size_test * sizeof(float));
    float *b = (float*)malloc(size_test * sizeof(float));
    float *c = (float*)malloc(size_test * sizeof(float));


    float *c_host = (float*)malloc(size_test * sizeof(float));



    float *device_c;
    cudaMalloc((void**)&device_c,size_test * sizeof(float));

    for(int i = 0;i<size_test;i++)
    {
        a[i] = rand() % size_test;
        b[i] = rand() % size_test;
        c[i] = a[i] + b[i];
    }

    float *device_a;
    cudaMalloc((void**)&device_a,size_test * sizeof(float));
    cudaMemcpy(device_a,a,size_test * sizeof(float),cudaMemcpyHostToDevice);

    float *device_b;
    cudaMalloc((void**)&device_b,size_test * sizeof(float));
    cudaMemcpy(device_b,b,size_test * sizeof(float),cudaMemcpyHostToDevice);

    int threadsPerBlock = 256;
    int blocksPerGrid = (size_test + threadsPerBlock - 1) / threadsPerBlock;

    vec_gpu_add<<<blocksPerGrid,threadsPerBlock>>>(device_a,device_b,device_c,size_test);

    cudaMemcpy(c_host,device_c,size_test * sizeof(float),cudaMemcpyDeviceToHost);

    for(int i = 0;i<size_test;i++)
    {
        if(c[i] != c_host[i])
        {
            std::cout<<"error"<<std::endl;
            break;
        }
        else
        {
            printf("%d=======%0.3f=============ok\n",i,c[i]);
        }
    }
    return 0;
}