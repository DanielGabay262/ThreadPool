#ifndef __ILRD_RD1167_THREADPOOL_HPP__
#define __ILRD_RD1167_THREADPOOL_HPP__

#include <thread>  //thread
#include <boost/interprocess/sync/interprocess_semaphore.hpp> //semaphore

#include "priority_queue.hpp"
#include "waitable_queue.hpp"
#include "task.hpp"


namespace ilrd
{


template<typename T> 
struct CompareLess
{   
    bool operator()( const T& lhs, const T& rhs ) const
    {
        return ((*lhs) < (*rhs));
    }
};
////////////////////////////////ThreadPool////////////////////////////////////////////////////////////
class ThreadPool
{
public:
    using TaskQueue = WaitableQueue<PriorityQueue<std::shared_ptr<Task>, std::vector<std::shared_ptr<Task>>,CompareLess<std::shared_ptr<Task>>>>; 
    explicit ThreadPool(size_t thread_num = std::thread::hardware_concurrency()); // default value may be 1
    ~ThreadPool() noexcept;
    ThreadPool(const ThreadPool &other) = delete;
    ThreadPool &operator=(const ThreadPool &other) = delete;
    void AddTask(std::shared_ptr<Task> new_task);
    void SetNumOfThreads(size_t num_threads);
    void Stop();
    void Pause();
    void Continue();

private:
    size_t m_thread_num;
    TaskQueue m_queue;
    std::vector<std::thread> m_threads;
    void RunThread();
    void InvokeThread(size_t thread_num);
    //std::shared_ptr<std::mutex> m_mutex;
    //std::shared_ptr<std::condition_variable_any> m_cond;
    std::shared_ptr<boost::interprocess::interprocess_semaphore> m_sem;
    bool m_is_paused;
};






}//ilrd

#endif//__ILRD_RD1167_THREADPOOL_HPP__ 
    