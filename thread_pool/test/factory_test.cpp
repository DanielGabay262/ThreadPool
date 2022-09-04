#include <iostream>

#include "factory.hpp"


class A
{
public:
    A(int a_, float b_) : a(a_), b(b_){}
    static std::shared_ptr<A> AFunc(int a_, float b_)
    {
        return std::make_shared<A>(a_, b_);
    }

private:
    int a;
    float b;
};


class B : public A
{
public:
    B(int c_) : A(1, 2.7), c(c_) {}
    static std::shared_ptr<B> Bfunc(int c_)
    {
        return std::make_shared<B>(c_);
    }

private:
    int c;
};


using namespace ilrd;

int main()
{
    Factory<int, A, int, float> f;

    f.Add(1, A::AFunc);
    std::shared_ptr<A> sp = f.Create(1, 1, 2.7);

    Factory<int, B, int> f2;
    f2.Add(1, B::Bfunc);
    std::shared_ptr<B> sp2 = f2.Create(1 ,3);

    return 0;

}