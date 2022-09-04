#ifndef __ILRD_RD1167_TASK_HPP__
#define __ILRD_RD1167_TASK_HPP__

#include <boost/interprocess/sync/interprocess_semaphore.hpp> //semaphore
#include <chrono>


namespace ilrd
{

////////////////////////////////Task////////////////////////////////////////////////////////////
class Task
{
public:
    enum Priority {LOW, MID, HIGH};
    Task(Priority priority = MID);
    virtual ~Task() = default;
    Task(const Task& other) = delete;
    Task& operator=(const Task& other) = delete;
    bool operator<(const Task& other);
    bool operator>(const Task& other);
    bool operator<=(const Task& other);
    bool operator==(const Task& other);
    friend class ThreadPool;

private:
    virtual void Execute() = 0;  // virtual void operator()() = 0; 
    Priority m_priority;
};



////////////////////////////////FutureTask////////////////////////////////////////////////////////////
template<typename T, typename ...Args>
class FutureTask: public Task
{
public:
    using func_t = std::function<T (Args...)>;
    explicit FutureTask(func_t func_ptr, Priority priority, Args ... args);
    ~FutureTask() = default;
    FutureTask(const FutureTask& other) = delete;
    FutureTask& operator=(const FutureTask& other) = delete;
    T GetReturnValue();
private:
    std::function<T (void)> m_ptr;
    T m_ret;
    boost::interprocess::interprocess_semaphore m_sem;
    void Execute();  // void operator()();
};




////////////////////////////////FutureTask implementations/////////////////////////////////////////////////////
template<typename T, typename ...Args>
FutureTask<T, Args...>::FutureTask(func_t func_ptr, Priority priority, Args ... args) : Task(priority), m_ptr([=](){return func_ptr(args...);}), m_ret(0), m_sem(0)
{

}

template<typename T, typename ...Args>
T FutureTask<T, Args...>::GetReturnValue()
{
    m_sem.wait();
    return m_ret;
}


template<typename T, typename ...Args>
void FutureTask<T, Args...>::Execute()
{
    m_ret = m_ptr();
    m_sem.post();
}


}//ilrd

#endif//__ILRD_RD1167_TASK_HPP__