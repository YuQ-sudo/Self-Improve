#ifndef _FRAMEWORK_INCLUDE_PIPLINE_HPP_
#define _FRAMEWORK_INCLUDE_PIPLINE_HPP_
//system include
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "set"
#include "memory"
#include "thread"
//
#include "config.hpp"
#include "module.hpp"
#include "connector.hpp"
enum CwStreamErrorCodeDDR
{
    cw_stream_error_ok = 0,
    cw_stream_module_create_failure,
    cw_stream_module_associate_failure,
    cw_stream_pipline_create_failure

};
enum AnalyzeMode{
    panoramic = 0,  //图片模式全景图
    snap      = 1,  //图片模式抠图
    video     = 2   //视频模式
};

struct ModuleAssociatedInfo {
    uint32_t parallelism = 0;
    std::shared_ptr<Connector> connector;
    std::set<std::string> down_nodes;
    std::vector<std::string> input_connectors;
    std::vector<std::string> output_connectors;
};
//定义一个pipline
class Pipline
{

public:
    explicit Pipline(const std::string& name);
    ~Pipline();

//对外调用接口
public:
    bool Start();
    void Stop();
    CwStreamErrorCodeDDR BuildPipeline(std::vector<ModuleConfig> &module_configs);
    void AddModuleConfig(const ModuleConfig& config);
    std::string LinkModules(std::shared_ptr<Module> up_node, std::shared_ptr<Module> down_node);
    inline bool IsRunning() const { return running_; }
    void TaskLoop(std::string node_name, uint32_t conveyor_idx);
private:
    std::string _name;
    //创建线程
    std::vector<std::thread> threads_;
    std::unordered_map<std::string, std::shared_ptr<Module>> modules_map_;
    std::unordered_map<std::string, std::shared_ptr<Connector>> links_;
    std::unordered_map<std::string, ModuleAssociatedInfo> modules_;
    std::unordered_map<std::string, ModuleConfig> modules_config_;
    std::unordered_map<std::string, std::vector<std::string>> connections_config_;

    std::atomic<bool> running_{false};


};
#endif // !_FRAMEWORK_INCLUDE_PIPLINE_HPP_