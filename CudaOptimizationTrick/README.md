# GitHub Link

<https://github.com/PacktPublishing/Learn-CUDA-Programming>

<https://github.com/nvidia/cuda-samples>

<https://github.com/ArchaeaSoftware/cudahandbook>

## GEMM 优化

* 优化手段：数据分块 以及 多级存储数据搬运
* 注意事项：bank conflict
  * 同时有两个或者多个线程存取同一个block数据时候发生冲突；
  * wrap在执行时候以half-wrap方式执行。不同wrap中的线程不存在bank冲突。多线程同时访问同一个bank的同一个地址时候不存在bank 冲突，这种称为广播机制。
  * 改变数据存储方式可避免bank 冲突；
  * bank conflict的影响：多个请求落在同一个bank内，只能响应一次请求，导致请求被序列化。
  * https://blog.csdn.net/feng__shuai/article/details/124323779
