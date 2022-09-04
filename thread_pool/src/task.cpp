#include <boost/interprocess/sync/interprocess_semaphore.hpp> //semaphore
#include <iostream>
#include <chrono>
#include "task.hpp"

using namespace ilrd;

////////////////////////////////Task implementations/////////////////////////////////////////////////////
Task::Task(Priority priority) : m_priority(priority)
{  
}

bool Task::operator>(const Task& other)
{
    std::cout << m_priority << std::endl;
    std::cout << other.m_priority << std::endl;
    return m_priority > other.m_priority;
}


bool Task::operator<(const Task& other)
{
    return m_priority < other.m_priority;
}

bool Task::operator<=(const Task& other)
{
    std::cout << m_priority << std::endl;
    std::cout << other.m_priority << std::endl;
    return m_priority <= other.m_priority;
}


bool Task::operator==(const Task& other)
{
    return m_priority == other.m_priority;
}