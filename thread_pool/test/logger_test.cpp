#include <iostream> // for prints
#include <string>   // for string


#include "logger.hpp" // logger header
#include "singleton.hpp" //


using namespace ilrd;


void Test()
{
    Logger& log = Singleton<Logger>::GetInstance();

    for(int i = 0; i < 5; ++i)
    {
        log.Write("Check1" , log.INFO, __LINE__, "test");
        log.Write("Check2" , log.INFO, __LINE__, "test");
        log.Write("Check3" , log.INFO, __LINE__, "test");
    }


}

int main()
{

    Test();
}