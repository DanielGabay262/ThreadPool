#include <iostream>

#include "singleton.hpp"
#include "factory.hpp"


using namespace ilrd;

class X
{
    friend class Singleton<X>;
    X(){};
};


int main()
{
    X& x1 = Singleton<X>::GetInstance();
    Factory<int, X> &f1 = Singleton<Factory<int, X> >::GetInstance();

    return 0;
}