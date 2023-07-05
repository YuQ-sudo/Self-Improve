#include "cuda_runtime.h"
#include "iostream"

void readNv12File(unsigned char* gpu_ptr,std::string src_file)
{
    std::ifstream nv12file(src_file.c_str(),std::ifstream::in | std::ios::binary);
    if(!nv12file.is_open()) return -1;
    int ctx_pitch = 1920;
    int width = 1920;
    int height = 1080;
    int ctx_alignment = 32;//线程束 32个线程
    int batch = 1;
    ctx_pitch += (ctx_pitch % ctx_alignment != 0)
                            ? (ctx_alignment - ctx_pitch % ctx_alignment)
                            : 0; //

    int ctx_heights = std::ceil(height * 3.0f / 2.0f);
    int length = ctx_pitch * ctx_heights;

    unsigned char* cpuNv12Data = (unsigned char *)malloc(length);
    nv12file.read((char *)cpuNv12Data,length);

    cudaMemcpy(void*(gpu_ptr),cpuNv12Data,length,cudaMemcpyHostToDevice);
    nv12file.close();
}

__global__ void nv12_bgr_kernel()
{



}

void nv12ToBgr(uint8_t *pNv12, int nNv12Pitch, float *pBgr,
    int nRgbPitch, int nWidth, int nHeight,
    int nBatchSize, cudaStream_t stream)
{
    
    //block size
    dim3 threads(32,16); 
    dim3 blocks((nWidth / 4 -1)/threads.x + 1,(nHeight / 2 - 1) / threads.y + 1, 1);

    nv12_bgr_kernel<<<blocks,threads,0>>>();


}
int main()
{
    cudaSetDevice(0);
    unsigned char *input_nv12;
    cudaMalloc((void **)&input_nv12,(1920*1080*3/2));
    readNv12File(input_nv12,"./test1920x1080.nv12");


}