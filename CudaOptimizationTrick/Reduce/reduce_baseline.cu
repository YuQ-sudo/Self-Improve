#include "cuda_runtime.h"
#include "stdlib.h"
#include <cuda.h>
#include <cassert>
#include <iostream>
#include <memory>

#include "device_launch_parameters.h"
#include <stdio.h>
#include <helper_functions.h> // for benchmark purpose

__global__ void reduce_baseline_kernel(float* d_in,float* d_out)
{
    int i = threadIdx.x + blockIdx.x*blockDim.x;
    int tid = threadIdx.x;
    __shared__ float sdata[512];
    //全局内存拷贝到共享内存
    sdata[tid] = d_in[i];
    __syncthreads();
    //一个block内数据进行处理 共享内存数据
    for(int s = 1; s < blockDim.x;s *= 2 )
    {
        if(tid % (2 * s) == 0)
        {
            sdata[tid] += sdata[tid + s];
        }
        __syncthreads();
    }
    // 结果数据
    if (tid == 0) d_out[blockIdx.x] = sdata[0];

}

__global__ void reduce_op1_kernel(float* d_in,float* d_out)
{
    int i = threadIdx.x + blockIdx.x*blockDim.x;
    int tid = threadIdx.x;
    __shared__ float sdata[512];
    //全局内存拷贝到共享内存
    sdata[tid] = d_in[i];
    __syncthreads();

    
    //一个block内数据进行处理 共享内存数据
    for(int s = 1; s < blockDim.x;s *= 2 )
    {
        int index = 2*s*tid;
        //优化过程 相当于减少了一半的wrap操作 相比较于base版本 但存在banck conflict
        if(index < blockDim.x){
            sdata[index]+=sdata[index+s];
        }
        __syncthreads();
    }
    // 结果数据
    if (tid == 0) d_out[blockIdx.x] = sdata[0];

}

__global__ void reduce_op2_kernel(float* d_in,float* d_out)
{
    int i = threadIdx.x + blockIdx.x*blockDim.x;
    int tid = threadIdx.x;
    __shared__ float sdata[512];
    //全局内存拷贝到共享内存
    sdata[tid] = d_in[i];
    __syncthreads();

    //一个block内数据进行处理 共享内存数据
    //消除bank
    //分析：0号线程 + 128线程 1号线程+129线程 第一个wrap 完成  不会 再冲突  新的wrap 32线程 + 160号线程
    for(unsigned int s=blockDim.x/2; s>0; s>>=1){
        if(tid < s){
            sdata[tid]+=sdata[tid+s];
        }
        __syncthreads();
    }
    
    // 结果数据
    if (tid == 0) d_out[blockIdx.x] = sdata[0];

}


int main()
{
    cudaSetDevice(0);

    const int64_t size = 32 * 1024 * 1024;
    float* a = (float *)malloc(size * sizeof(float));
    float* d_a;
    float* d_b;
    cudaMalloc((void**)&d_a,size * sizeof(float));

    double cpu_calc = 0;
    for(int64_t i = 0; i < size;i++)
    {
        a[i] = 1;
    }
    for(int64_t i = 0; i < size;i++)
    {
        // if(i % 1024 == 0)
        //     printf("a[%lld]:%0.3f %0.3f\n",i,a[i],cpu_calc);
        cpu_calc +=a[i];
    }

    cudaMemcpy(d_a,a,size*sizeof(float),cudaMemcpyHostToDevice);

    int block_per_threads = 512;

    int num_blocks = (size - 1) / block_per_threads + 1;

    cudaMalloc((void**)&d_b,(num_blocks) * sizeof(float));
    float *res=(float *)malloc((num_blocks)*sizeof(float));

    dim3 grid(num_blocks,1);

    dim3 block(block_per_threads,1);

    StopWatchInterface *timer = NULL;
    float elapsedTimeInMs = 0.0f;
    // float bandwidthInGBs = 0.0f;
    sdkCreateTimer(&timer);
    cudaEvent_t start, stop;
    (cudaEventCreate(&start));
    (cudaEventCreate(&stop));
    int MEMCOPY_ITERATIONS = 1;
    elapsedTimeInMs = 0;
    for (unsigned int i = 0; i < MEMCOPY_ITERATIONS; i++) {
      sdkStartTimer(&timer);
      reduce_baseline_kernel<<<grid,block>>>(d_a,d_b);

      
      sdkStopTimer(&timer);
      elapsedTimeInMs = sdkGetTimerValue(&timer);
      printf("time cost base:%0.3f ms\n",elapsedTimeInMs);
      sdkResetTimer(&timer);

      sdkStartTimer(&timer);
      reduce_op1_kernel<<<grid,block>>>(d_a,d_b);
      sdkStopTimer(&timer);
      elapsedTimeInMs = sdkGetTimerValue(&timer);
      printf("time cost op1:%0.3f ms\n",elapsedTimeInMs);
      sdkResetTimer(&timer);

      sdkStartTimer(&timer);
      reduce_op2_kernel<<<grid,block>>>(d_a,d_b);
      sdkStopTimer(&timer);
      elapsedTimeInMs = sdkGetTimerValue(&timer);
      printf("time cost op2:%0.3f ms\n",elapsedTimeInMs);
      sdkResetTimer(&timer);
    //   memset(flush_buf, i, FLUSH_SIZE);
    }
    cudaMemcpy( res, d_b, (num_blocks)*sizeof(float), cudaMemcpyDeviceToHost);


    for(int i = 1; i < num_blocks;i++)
    {
        res[0] += res[i];
    }
    printf("%0.3f %0.3f %0.3f\n",cpu_calc,res[0],res[num_blocks-1]);
    // double time_s = elapsedTimeInMs / 1e3;
    // bandwidthInGBs = (size * float(MEMCOPY_ITERATIONS)) / (double)1e9;
    // bandwidthInGBs = bandwidthInGBs / time_s;
    // printf("bandth:%0.3f GBs\n",bandwidthInGBs);



}




