#include <memory>

#include "pipline.hpp"


Pipline::Pipline(const std::string& name) : _name(name){


};
Pipline::~Pipline(){


};

void Pipline::TaskLoop(std::string node_name, uint32_t conveyor_idx)
{
    std::shared_ptr<Module> instance = modules_map_[node_name];
    while(1)
    {
        //每个connector 内部的开始pop数据
    }
};

bool Pipline::Start(){
    if(IsRunning()) return true;

    //如果出现open fail 需要调用close函数
    for(auto& itr : modules_map_)
    {
        itr.second->Open(modules_config_[itr.first].parameters);
    }

    running_.store(true);

    for(auto& itr : modules_)
    {
        for(int conveyor_idx = 0;conveyor_idx < itr.second.parallelism;conveyor_idx++)
        {
            threads_.push_back(std::thread(&Pipeline::TaskLoop, this, itr.first, conveyor_idx));
        }
    }




};
void Pipline::Stop(){

};

void Pipline::AddModuleConfig(const ModuleConfig& config) {
  modules_config_[config.name] = config;
  connections_config_[config.name] = config.next;
};
std::string Pipline::LinkModules(std::shared_ptr<Module> up_node, std::shared_ptr<Module> down_node)
{
    std::string up_name = up_node->GetModuleName();
    std::string down_name = down_node->GetModuleName();

    ModuleAssociatedInfo& up_node_info = modules_.find(up_name)->second;
    ModuleAssociatedInfo& down_node_info = modules_.find(down_name)->second;

    std::string link_id = up_node->GetModuleName() + "-->" + down_node->GetModuleName();
    if(!down_node_info.connector)
    {
        printf("down node connector has not been create\n");
        return " ";
    }
    auto ret = up_node_info.down_nodes.insert(down_name);
    if(!ret.second)
    {
        return link_id;
    }
    up_node_info.output_connectors.push_back(link_id);

    down_node_info.input_connectors.push_back(link_id);

    links_[link_id] = down_node_info.connector;

    return link_id;

}
CwStreamErrorCodeDDR Pipline::BuildPipeline(std::vector<ModuleConfig> &module_configs){

    //创建module
    ModuleCreatorWorker creator;
    std::vector<std::shared_ptr<Module>> modules;
    for(auto & config : module_configs)
    {

        AddModuleConfig(config);
        // std::cout<<config.className <<"-"<< config.name<<std::endl;
        // 创建各个模块的实例
        std::shared_ptr<Module> instance(creator.Create(config.className, config.name));
        if(config.parallelism <= 0) config.parallelism = 1;
        if(config.maxInputQueueSize <= 0) config.maxInputQueueSize = 10;

        //module name做映射存储
        ModuleAssociatedInfo associated_info;
        associated_info.parallelism = config.parallelism;
        // associated_info.down_nodes = config.next;
        associated_info.connector = std::make_shared<Connector>(config.parallelism,config.maxInputQueueSize);
        modules_.insert(std::make_pair(config.name, associated_info));
        modules_map_.insert(std::make_pair(config.name,instance));
        modules.push_back(instance);
    }
    //module 进行link
    for(auto front : connections_config_)
    {
        for(auto back : front.second)
        {
            auto &up_node = modules_map_[front.first];
            auto &down_node = modules_map_[back];
            std::string link_route = LinkModules(up_node,down_node);
            printf("%s\n",link_route.c_str());
        }
    }
};




