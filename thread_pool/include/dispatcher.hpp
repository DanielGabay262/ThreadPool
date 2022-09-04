#ifndef __ILRD_RD1167_DISPATCHER_HPP__
#define __ILRD_RD1167_DISPATCHER_HPP__
 
#include<memory> 
#include<functional>
#include <iterator>
#include <set>


namespace ilrd
{

template<typename MESSAGE>
class CallbackBase;



///////////////////////////////////////////// Dispatcher CLASS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
template<typename MESSAGE>
class Dispatcher 
{
public:

    explicit Dispatcher() = default;
    ~Dispatcher();
    Dispatcher(const Dispatcher& other) = delete;
    Dispatcher& operator=(const Dispatcher& other) = delete;
    void NotifyAll(const MESSAGE& msg);
    friend class CallbackBase<MESSAGE>;

private:
    std::set<CallbackBase<MESSAGE> *> m_callback;
    void Subscribe(CallbackBase<MESSAGE> *callback);
    void UnSubscribe(CallbackBase<MESSAGE> *callback);

};


///////////////////////////////////////////// CallbackBase CLASS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
template<typename MESSAGE>
class CallbackBase
{
public:
    explicit CallbackBase(Dispatcher<MESSAGE> *dispatcher);
    virtual ~CallbackBase();
    CallbackBase(const CallbackBase &other) = delete;
    CallbackBase& operator=(const CallbackBase& other) = delete;
    virtual void Act(const MESSAGE& msg) = 0;
    virtual void Stop() = 0;
private:
    Dispatcher<MESSAGE> *m_dispatcher;
};


///////////////////////////////////////////// Callback CLASS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
template<typename MESSAGE, typename T>
class Callback : public CallbackBase<MESSAGE>
{
public:

    explicit Callback(Dispatcher<MESSAGE> *dispatcher, T &obj ,std::function<void(T *const this_obj, const MESSAGE& msg)> act, std::function<void(T *const this_obj)> stop = nullptr);
    ~Callback();
    Callback(const Callback& other) = delete;
    Callback& operator=(const Callback& other) = delete;
    void Act(const MESSAGE& msg);
    void Stop();

private:
    T &m_obj;
    std::function<void(T *const this_obj, const MESSAGE& msg)> m_act;
    std::function<void(T *const this_obj)> m_stop;

};




///////////////////////////////////////////// Dispatcher implementations \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
template<typename MESSAGE>
void Dispatcher<MESSAGE>::NotifyAll(const MESSAGE& msg)
{
    for (auto it = m_callback.begin(); it != m_callback.end(); ++it)
    {
        (*it)->Act(msg);
    }
}


template<typename MESSAGE>
Dispatcher<MESSAGE>::~Dispatcher()
{
    for (auto it = m_callback.begin(); it != m_callback.end(); ++it)
    {
        (*it)->Stop();
    }
}



template<typename MESSAGE>
void Dispatcher<MESSAGE>::Subscribe(CallbackBase<MESSAGE> *callback)
{
    m_callback.insert(callback);
}



template<typename MESSAGE>
void Dispatcher<MESSAGE>::UnSubscribe(CallbackBase<MESSAGE> *callback)
{
    m_callback.erase(callback);
}



///////////////////////////////////////////// CallbackBase implementations \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
template<typename MESSAGE>
CallbackBase<MESSAGE>::CallbackBase(Dispatcher<MESSAGE> *dispatcher) : m_dispatcher(dispatcher)
{
    m_dispatcher->Subscribe(this);
}


template<typename MESSAGE>
CallbackBase<MESSAGE>::~CallbackBase()
{
    m_dispatcher->UnSubscribe(this);
}


///////////////////////////////////////////// Callback implementations \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
template<typename MESSAGE, typename T>
Callback<MESSAGE, T>::Callback(Dispatcher<MESSAGE> *dispatcher, T &obj ,std::function<void(T *const this_obj, const MESSAGE& msg)> act, std::function<void(T *const this_obj)> stop) :
                                                                                                                                CallbackBase<MESSAGE>(dispatcher),
                                                                                                                                m_obj(obj),
                                                                                                                                m_act(act),
                                                                                                                                m_stop(stop)
{
}


template<typename MESSAGE, typename T>
Callback<MESSAGE, T>::~Callback()
{
    Stop();
}


template<typename MESSAGE, typename T>
void Callback<MESSAGE, T>::Act(const MESSAGE& msg)
{
    m_act(&m_obj, msg);
}


template<typename MESSAGE, typename T>
void Callback<MESSAGE, T>::Stop()
{
    if (m_stop != nullptr)
    {
        m_stop(&m_obj);
    }
}



}
#endif //__ILRD_RD1167_DISPATCHER_HPP__