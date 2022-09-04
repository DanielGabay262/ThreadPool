//#include <iostrean>
#include <thread>  //thread
#include <boost/interprocess/sync/interprocess_semaphore.hpp> //semaphore

#include "threadpool.hpp"
#include "priority_queue.hpp"
#include "waitable_queue.hpp"
#include "task.hpp"


using namespace ilrd;


////////////////////////////////BadApple////////////////////////////////////////////////////////////
class BadApple : public std::exception
{
public:
    inline const char *what();
};

inline const char *BadApple::what()
{
    return "stop thread!!!";
}



////////////////////////////////StopTask/////////////////////////////////////////////////////
class StopTask : public Task
{
public:
    StopTask();
private:
    void Execute();
};


StopTask::StopTask() : Task(static_cast<Priority>(HIGH + 2))
{

}

void StopTask::Execute()
{
    throw BadApple();
}



////////////////////////////////PauseTask/////////////////////////////////////////////////////
class PauseTask : public Task
{
public:
    //PauseTask(std::shared_ptr<std::mutex> mutex, std::shared_ptr<std::condition_variable_any> cv);
    PauseTask(std::shared_ptr<boost::interprocess::interprocess_semaphore> sem);
private:
    //std::shared_ptr<std::mutex> m_mutex;
    //std::shared_ptr<std::condition_variable_any> m_cond;
    std::shared_ptr<boost::interprocess::interprocess_semaphore> m_sem;
    void Execute();
};

//PauseTask::PauseTask(std::shared_ptr<std::mutex> mutex, std::shared_ptr<std::condition_variable_any> cv) : Task(SUPER_HIGH), m_mutex(mutex), m_cond(cv){}
PauseTask::PauseTask(std::shared_ptr<boost::interprocess::interprocess_semaphore> sem) : Task(static_cast<Priority>(HIGH + 1)), m_sem(sem){}


void PauseTask::Execute()
{
    m_sem->wait();
}



//////////////////////////////ThreadPool/////////////////////////////////////////////////////
// ThreadPool::ThreadPool(size_t thread_num) : m_thread_num(thread_num), m_mutex(std::make_shared<std::mutex>()), m_cond(std::make_shared<std::condition_variable_any>()), is_paused(false)
// {
//     InvokeThread(thread_num);
// }

ThreadPool::ThreadPool(size_t thread_num) : m_thread_num(thread_num), m_sem(std::make_shared<boost::interprocess::interprocess_semaphore>(0)), m_is_paused(false)
{
    InvokeThread(thread_num);
}





void ThreadPool::AddTask(std::shared_ptr<Task> new_task)
{
    m_queue.Push(new_task);
}


void ThreadPool::RunThread()
{
    std::shared_ptr<Task> task;

    while (1)
    {
        m_queue.Pop(task);
        try
        {
            task->Execute();
            
        }
        catch(BadApple& e)
        {
            return;
        }
    }
}


void ThreadPool::InvokeThread(size_t thread_num)
{
    for (size_t i = 0; i < thread_num; ++i)
    {
        m_threads.push_back(std::thread([&](){RunThread();}));
    }
}


ThreadPool::~ThreadPool()
{
    Stop();
}


void ThreadPool::Stop()
{
    std::shared_ptr<Task> task = std::make_shared<StopTask>();
    for (size_t i = 0; i < m_thread_num; ++i)
    {
        AddTask(task);
    }
    for (size_t i = 0; i < m_thread_num; ++i)
    {

        m_threads[i].join();
    }
    m_threads.clear();
    m_thread_num = 0;
}




void ThreadPool::Pause()
{
    if (m_is_paused == false)
    {
        std::shared_ptr<Task> task = std::make_shared<PauseTask>(m_sem);
        for (size_t i = 0; i < m_thread_num; ++i)
        {
            AddTask(task);
        }
        m_is_paused = true;
    }
}


void ThreadPool::Continue()
{
    if (m_is_paused == true)
    {
        for (size_t i = 0; i < m_thread_num; ++i)
        {
            m_sem->post();
        }
    }
    m_is_paused = false;
}

void ThreadPool::SetNumOfThreads(size_t num_threads)
{
    if (num_threads >= m_thread_num)
    {
        InvokeThread(num_threads - m_thread_num);
    }
    else
    {
        Stop();
        InvokeThread(num_threads);
    }
    m_thread_num = num_threads;
}


