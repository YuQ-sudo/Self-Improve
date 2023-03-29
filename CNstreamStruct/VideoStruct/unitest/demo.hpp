#pragma once
#include "pipline.hpp"

class DataSource : public Module,public ModuleCreator<DataSource>
{
public:
    //super 本类的基类 
    explicit DataSource(const std::string &name) : Module(name) {};
    void Open(ModuleParamSet param_set)
    {

    };
    void Close()
    {

    };



};

class Detection : public Module,public ModuleCreator<Detection>
{
public:
        //super 本类的基类 
    explicit Detection(const std::string &name) : Module(name) {};
    void Open(ModuleParamSet param_set)
    {
        
    };
    void Close()
    {

    };



};
class Track : public Module,public ModuleCreator<Track>
{
    public:
            //super 本类的基类 
    explicit Track(const std::string &name) : Module(name) {};
    void Open(ModuleParamSet param_set)
    {
        
    };
    void Close()
    {

    };

};