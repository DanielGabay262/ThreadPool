#ifndef __ILRD_RD1167_SINGLETON_HPP__
#define __ILRD_RD1167_SINGLETON_HPP__

#include <iostream>
#include <string>

namespace ilrd
{

template <typename T>
class Singleton
{
public:
    Singleton(const Singleton& other_) = delete;
    Singleton& operator=(const Singleton& other_) = delete;
    ~Singleton() = default;
    static T &GetInstance();
private:
    Singleton(const T &var);
};



template <typename T>
T &Singleton<T>::GetInstance()
{
    static T s;
    return s;
}


template <typename T>
Singleton<T>::Singleton(const T &var){}


}//ilrd




#endif //__ILRD_RD1167_SINGLETON_HPP__