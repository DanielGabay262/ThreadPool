#ifndef __ILRD_RD1167_WAITABLE_QUEUE_HPP__
#define __ILRD_RD1167_WAITABLE_QUEUE_HPP__

#include <chrono> // for milliseconds
#include <mutex>  // for mutex
#include <condition_variable> // for condition variable

namespace ilrd
{
    template<typename Container>
    using ValueType = typename Container::value_type;



/***************************************************************************
 *                         WaitableQueue calss
*************************************************************************/
template<typename Container>
class WaitableQueue 
{
public:
    explicit WaitableQueue() = default;
    ~WaitableQueue() = default;
    WaitableQueue(const WaitableQueue &other_) = delete;
    WaitableQueue& operator=(const WaitableQueue &other_) = delete;


    void Push(ValueType<Container>& value_) noexcept;
    bool Pop(ValueType<Container>& out_, std::chrono::milliseconds time_out) noexcept;
    void Pop(ValueType<Container>& out_) noexcept;
    void Peek(ValueType<Container>& out_) const noexcept;
    bool IsEmpty(void) const noexcept;

private:
    void LockAndWait(ValueType<Container>& out_, std::unique_lock<std::recursive_mutex> &lock) const noexcept;
    Container m_container;
    mutable std::recursive_mutex m_lock; // use lock_guard
    mutable std::condition_variable_any m_cv;

};



/***************************************************************************
 *                         implementations
*************************************************************************/
template<typename Container>
void WaitableQueue<Container>::Push(ValueType<Container>& value_) noexcept
{
    std::unique_lock<std::recursive_mutex> lock(m_lock);
    m_container.push(value_);
    m_cv.notify_one();
}

template<typename Container>
bool WaitableQueue<Container>::Pop(ValueType<Container>& out_, std::chrono::milliseconds time_out) noexcept
{
    std::unique_lock<std::recursive_mutex> lock(m_lock);
    if (m_cv.wait_for(lock, time_out, [&]{return (IsEmpty() == false);}) == true)
    {
        out_ = m_container.front();
        m_container.pop();
        return true;
    }
    return false;
}

template<typename Container>
void WaitableQueue<Container>::LockAndWait(ValueType<Container>& out_, std::unique_lock<std::recursive_mutex> &lock) const noexcept
{
    m_cv.wait(lock, [&]{return (IsEmpty() == false);});
    out_ = m_container.front();
}


template<typename Container>
void WaitableQueue<Container>::Pop(ValueType<Container>& out_) noexcept
{
    std::unique_lock<std::recursive_mutex> lock(m_lock);
    LockAndWait(out_, lock);
    m_container.pop();
}

template<typename Container>
void WaitableQueue<Container>::Peek(ValueType<Container>& out_) const noexcept
{
    std::unique_lock<std::recursive_mutex> lock(m_lock);
    LockAndWait(out_, lock);
}

template<typename Container>
bool WaitableQueue<Container>::IsEmpty() const noexcept
{
    std::unique_lock<std::recursive_mutex> lock(m_lock);
    return m_container.empty();     
}


} // ilrd namespace end

#endif //__ILRD_RD1167_WAIATABLE_QUEUE_HPP__



