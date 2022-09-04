#include <mutex>

#include "singleton.hpp"

using namespace ilrd;

std::shared_ptr<Singleton> Singleton::m_instance = nullptr;
std::mutex Singleton::m_mutex;

std::shared_ptr<Singleton> Singleton::GetInstance()
{
    if (m_instance == nullptr)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_instance == nullptr)
        {
            std::shared_ptr<Singleton> instance(new Singleton);
            m_instance = instance;
        }
    }
    return m_instance;
}


