#include <iostream>

#include "dispatcher.hpp"

using namespace ilrd;


class WaitForString
{
public:
    WaitForString(const std::string& str = "") : m_string(str){}
    void SetString(const std::string& string)
    {
        m_string = string;
    }
    void Stop()
    {
    }

    std::string GetString()
    {
        return m_string;
    }
private:
    std::string m_string;
};

int main()
{
    Dispatcher<std::string> dispatcher;
    WaitForString wait_for_string1;
    WaitForString wait_for_string2("TOMER THE SNAKE");
    WaitForString wait_for_string3("BLA");

    if (wait_for_string1.GetString().compare("") != 0)
    {
        std::cout << "error 1" << std::endl;
    }

    if (wait_for_string2.GetString().compare("TOMER THE SNAKE") != 0)
    {
        std::cout << "error 2" << std::endl;
    }

    if (wait_for_string3.GetString().compare("BLA") != 0)
    {
        std::cout << "error 3" << std::endl;
    }
    
    Callback<std::string, WaitForString> callback(&dispatcher, wait_for_string1, &WaitForString::SetString, &WaitForString::Stop);
    Callback<std::string, WaitForString> callback2(&dispatcher, wait_for_string2, &WaitForString::SetString, &WaitForString::Stop);
    Callback<std::string, WaitForString> callback3(&dispatcher, wait_for_string3, &WaitForString::SetString, &WaitForString::Stop);

    dispatcher.NotifyAll("Start");

    if (wait_for_string1.GetString().compare("Start") != 0)
    {
        std::cout << "error 4" << std::endl;
    }

    if (wait_for_string2.GetString().compare("Start") != 0)
    {
        std::cout << "error 5" << std::endl;
    }

    if (wait_for_string3.GetString().compare("Start") != 0)
    {
        std::cout << "error 6" << std::endl;
    }

    

}
