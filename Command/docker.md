# docker 常见问题以及解决方法

## docker 安装

* 详见docker_install.md

## docker 指令相关

* docker push出现错误：denied: requested access to the resource is denied
  解决方案：<https://www.cnblogs.com/jcjc/p/11764970.html>
  push远程仓库之前首先进行docker login + 远程地址

* docker 更改images内容并保存更新到远程仓库
    docker commit -m "alg demo tool base proj" -a zyq 1a555dc799a5 10.128.2.6/zyq/headcount:v0.1
    docker push 10.128.2.6/zyq/headcount:v0.1

* docker 保存镜像以及加载镜像指令
    docker save -o alg_head_count.tar 10.128.2.6/zyq/headcount:v0.1  
    docker load -i alg_head_count.tar

* docker中“ptrace: Operation not permitted.”的处理方法：
    docker 创建时候加上--cap-add=SYS_PTRACE

* docker创建容器指令暴露22端口号
  * docker run -it -P --cap-add=SYS_PTRACE --expose 22 --gpus all --shm-size 32G --name zhangyuqi0 -v /home/yckj2733/:/home 10.128.2.6/qinqin/mmdet-detectron2:v0.6 /bin/bash
  * docker run -it -p xxxxx:22 --security-opt seccomp=unconfined --runtime nvidia -e NVIDIA_DRIVER_CAPABILITIES=video,compute,utility --name xxxxx -v /xxxxxx:/xxxxxx artifact.cloudwalk.work/rd_docker_release/trtis/triton-dev:1.0.10

* docker 创建atlas容器指令davinci31为使用的芯片id
    docker run -it --name atlas-tis --cap-add LINUX_IMMUTABLE --device=/dev/davinci31 --device=/dev/davinci_manager --device=/dev/hisi_hdc --device=/dev/devmm_svm -v /usr/local/sbin/npu-smi:/usr/local/sbin/npu-smi -v /home/yckj1195/tis_backend/:/home/tis_backend/ atlas/tis:V1.0 /bin/bash

* 启动Docker 守护进程---> docker启动服务---->查看服务 重启服务
    systemctl daemon-reload
    systemctl start docker
    ps -ef |grep docker
    systemctl restart docker
* docker inspect xxx | grep "xxx" 查询docker的映射以及log信息等
* docker exec -it xxx /bin/bash 进入docker
