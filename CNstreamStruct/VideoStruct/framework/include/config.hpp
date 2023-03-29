#ifndef _FRAMEWORK_INCLUDE_CONFIG_HPP_ 
#define _FRAMEWORK_INCLUDE_CONFIG_HPP_ 
#include <unistd.h>
#include <string.h>
#include <unordered_map>
#include <vector>

using ModuleParamSet = std::unordered_map<std::string, std::string>;

struct ModuleConfig
{
    std::string name;//module name
    std::unordered_map<std::string,std::string> parameters;
    int parallelism;
    int maxInputQueueSize;
    std::string className;//module class name
    std::vector<std::string> next;//向下连接的节点

};




#endif