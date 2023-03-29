#ifndef _FRAMEWORK_INCLUDE_MODULE_HPP_
#define _FRAMEWORK_INCLUDE_MODULE_HPP_

#include <string.h>
#include <unordered_map>
#include <functional>
#include <vector>
#include <cxxabi.h>
#include <iostream>
//自动注册反射机制 可参考Nebula C++ 
class Module
{
public:
    explicit Module(const std::string& name) : _name(name){};
    //子类继承内存需要释放
    virtual ~Module(){};
    //module开启自己的线程
    virtual void Open(ModuleParamSet param_set) = 0;
    virtual void Close() = 0;
public:
    std::string GetModuleName(){return _name;};
private:
    std::string  _name;
};
// template <typename T> 


class ModuleFactory
{
public:

    virtual ~ModuleFactory(){};

    //创建实例
    static ModuleFactory* Instance()
    {
        if(module_factory == nullptr)
        {
            module_factory = new ModuleFactory();
        }
        return module_factory;
    };

    //注册
    bool Regist(const std::string &className, std::function<Module *(const std::string &)> pFunc)
    {

        if(pFunc == nullptr) return false;
     
        bool t = _map.insert(std::make_pair(className,pFunc)).second;

        return true;
    }

    Module *Create(const std::string &className, const std::string &name) {

        auto iter = _map.find(className);
        //查询module是否注册过
        if (iter == _map.end()) {

            return (nullptr);
        } else {

            return (iter->second(name));
        }
    }

    std::vector<std::string> GetAllRegisted()
    {
        std::vector<std::string> registed;
        for(auto& itr : _map)
        {
            registed.push_back(itr.first);
            // printf("regist module:%s\n",itr.first.c_str());
        }
        return registed;
    }
private:
    ModuleFactory(){};
    std::unordered_map<std::string,std::function< Module *(const std::string &) >> _map;
    static ModuleFactory* module_factory;
};


//子类定义 继承父类module时候 添加父类 ModuleCreator 进行注册.
template <typename T>
class ModuleCreator
{
public:
    struct Register
    {
        //构造函数
        Register()
        {
            char *szDemangleName = nullptr;
            std::string className;
            #ifdef __GNUC__
                szDemangleName = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
            #else
                // in this format?:     szDemangleName =  typeid(T).name();
                szDemangleName = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
            #endif
            if (nullptr != szDemangleName) {
                className = szDemangleName;
                free(szDemangleName);
            }
            ModuleFactory::Instance()->Regist(className,CreateObject);
        }
        inline void do_nothing() const {};

    };
    //实例化必然调用构造函数
public:
    static Register register_;
    static T *CreateObject(const std::string &name) { 
        return new T(name); }
public:
    //register_ donothing 使用了静态变量 确保首次使用和动态初始化的顺序
    ModuleCreator(){register_.do_nothing();};
    virtual ~ModuleCreator(){register_.do_nothing();};
};

template <typename T>
typename ModuleCreator<T>::Register ModuleCreator<T>::register_;

class ModuleCreatorWorker
{
public:
    Module *Create(const std::string &className, const std::string &name)
    {
   
        // ModuleFactory::Instance().GetAllRegisted();
        //工厂模式 父工厂实例创建各个module的实例
        return ModuleFactory::Instance()->Create(className,name);
    }

};


















#endif