#ifndef __ILRD_RD1167_DIRMONITOR_HPP__
#define __ILRD_RD1167_DIRMONITOR_HPP__

#include <thread>
#include <atomic>

#include "dispatcher.hpp"
#include "logger.hpp"

namespace ilrd
{

class DirMonitor
{
public:
    DirMonitor(const std::string &path, Logger& logger);
    ~DirMonitor() noexcept;
    DirMonitor(const DirMonitor& other) = delete;
    DirMonitor& operator=(const DirMonitor& other) = delete;
    Dispatcher<std::string> *GetDispatcher() noexcept;
private:
    void RunThread(const std::string& path);
    int m_inotify_fd;
    Dispatcher<std::string> m_dispatcher;
    Logger& m_log;
    std::thread m_thread;
    std::atomic<bool> m_is_running;
};


}//ilrd


#endif /* __ILRD_RD1167_DIRMONITOR_HPP__*/