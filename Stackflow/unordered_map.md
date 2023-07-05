# 接口函数

<https://legacy.cplusplus.com/reference/unordered_set/unordered_set/?kw=unordered_set>

* count()返回值为1或者0.一对多的映射 请使用unordered_multimap。
* find()返回迭代器。
* 相似的用法还有unordered_set

## mutimap遍历方式

```cpp
    std::multimap<int,int> test;
    typedef std::multimap<int,int> mult;
    
    for(auto& itr = test.begin();itr != test.end(); itr++)
    {
        std::pair<mult::iterator,mult::iterator> ret = test.equal_range(itr->first);
        for(auto & it = ret.begin(); it != ret.end();it++)
        {
            it->second;
        }
    }


```
