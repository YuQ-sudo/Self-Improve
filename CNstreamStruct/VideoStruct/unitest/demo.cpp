#include <iostream>

#include "demo.hpp"



int main()
{
    Pipline pipline("test");

    ModuleConfig moduleA;
    moduleA.name = "moduleA";
    moduleA.parallelism = 8;
    moduleA.maxInputQueueSize = 16;
    moduleA.className = "DataSource";
    moduleA.next.push_back("moduleB");
    moduleA.next.push_back("moduleC");

    ModuleConfig moduleB;
    moduleB.name = "moduleB";
    moduleB.parallelism = 8;
    moduleB.maxInputQueueSize = 16;
    moduleB.className = "Detection";
    moduleB.next.push_back("moduleD");

    ModuleConfig moduleC;
    moduleC.name = "moduleC";
    moduleC.parallelism = 8;
    moduleC.maxInputQueueSize = 16;
    moduleC.className = "Detection";
    moduleC.next.push_back("moduleD");

    ModuleConfig moduleD;
    moduleD.name = "moduleD";
    moduleD.parallelism = 8;
    moduleD.maxInputQueueSize = 16;
    moduleD.className = "Track";

    std::vector<ModuleConfig> module_configs;//{moduleA,moduleB,moduleC,moduleD};
    module_configs.push_back(moduleA);
    module_configs.push_back(moduleB);
    module_configs.push_back(moduleC);
    module_configs.push_back(moduleD);
    
    pipline.BuildPipeline(module_configs);
    // DataSource A("test");


}