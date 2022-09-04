#ifndef __ILRD_RD1167_FACTORY_HPP__
#define __ILRD_RD1167_FACTORY_HPP__

#include <unordered_map>
#include <functional>
#include <memory>



namespace ilrd
{
    
template<typename K, typename Base, typename ...Args>

class Factory
{
public:
    std::shared_ptr<Base> Create(K Key, Args... params);
    void Add(K Key, std::function <std::shared_ptr<Base>(Args... params)> func_ptr);
private:
    Factory(){}
    friend class Singleton<Factory>; 
    std::unordered_map<K, std::function <std::shared_ptr<Base>(Args... params)> >map;
};


template<typename Base, typename ...Args>
bool IsValidPtr(std::function <std::shared_ptr<Base>(Args... params)> func_ptr)
{
    return (func_ptr != nullptr);
}


template<typename K, typename Base, typename ...Args>
void Factory<K, Base, Args...>::Add(K Key, std::function <std::shared_ptr<Base>(Args... params)> func_ptr)
{
    if (IsValidPtr(func_ptr) == true)
    {
        map.insert({Key, func_ptr});
    }
}


template<typename K, typename Base, typename ...Args>
std::shared_ptr<Base> Factory<K, Base, Args...>::Create(K Key, Args... params)
{
    return map.at(Key)(params...);
}





} // ilrd namespace end



#endif //__ILRD_RD1167_FACTORY_HPP__
