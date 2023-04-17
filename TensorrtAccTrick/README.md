# learning root

<https://github.com/NVIDIA/trt-samples-for-hackathon-cn>

## Date 20230406

+ cookbook
  + SimpleDemo
    + Main Process

    ```cpp
        //onnx
        //创建构建器
        IBuilder *            builder = createInferBuilder(gLogger);
        //计算图
        INetworkDefinition *  network = builder->createNetworkV2(1U << int(NetworkDefinitionCreationFlag::kEXPLICIT_BATCH));
        //创建优化配置文件 用于输入输出张量
        IOptimizationProfile *profile = builder->createOptimizationProfile();
        IBuilderConfig *      config  = builder->createBuilderConfig();
        config->setMemoryPoolLimit(MemoryPoolType::kWORKSPACE, 1 << 30);
        //创建onnx解析器 构建计算图网络
        nvonnxparser::IParser parser = (nvonnxparser::createParser(*network, sample::gLogger.getTRTLogger()));  
        auto parsed = parser->parseFromFile("onnx_path");
        config->setMaxWorkspaceSize(16_MiB);
        config->setFlag(BuilderFlag::kFP16);

        IHostMemory *engineString = builder->buildSerializedNetwork(*network, *config);
        //创建序列化文件
        IRuntime *runtime {createInferRuntime(gLogger)};
        engine = runtime->deserializeCudaEngine(engineString->data(), engineString->size());
        if (engine == nullptr)
        {
            std::cout << "Failed building engine!" << std::endl;
            return;
        }
        std::cout << "Succeeded building engine!" << std::endl;
        //序列化的文件保存在trtFile里面
        std::ofstream engineFile(trtFile, std::ios::binary);
        if (!engineFile)
        {
            std::cout << "Failed opening file to write" << std::endl;
            return;
        }
        engineFile.write(static_cast<char *>(engineString->data()), engineString->size());
        if (engineFile.fail())
        {
            std::cout << "Failed saving .plan file!" << std::endl;
            return;
        }
        std::cout << "Succeeded saving .plan file!" << std::endl;
        //创建上下文
        IExecutionContext *context = engine->createExecutionContext();

        //图像前处理
        ...
        //推理
        context->executeV2(buffers.getDeviceBindings().data());
    ```

## Date 20230407

<https://zhuanlan.zhihu.com/p/531730491>

* net layer
  * activation layer:每一层的输入输出都是线性求和的过程，如果没有激活函数会导致整个神经网络是一个线性组合系统。
  * assertion layer: 用于校验核对网络输出的维度是否一致
  * concatenation layer：维度变化网络层 可以将两个tensor 拼接为一个维度
  * constant layer：给定维度d和权重向量w ，Constant层将输出维度为d的张量B ，其中常数值在w中。该层不接受输入张量。权重向量w中的元素数量等于d的volume。
  * IIdentityLayer: 该层的输出在数学上与输入相同。该层允许您精确控制张量的精度并从一种精度转换为另一种精度。如果输入的精度与输出的精度不同，则该层会将输入张量转换为输出精度。

