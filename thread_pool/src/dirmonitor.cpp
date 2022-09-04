#include <sys/inotify.h>

#include "dirmonitor.hpp"
#include "singleton.hpp"
#include "logger.hpp"


using namespace ilrd;


DirMonitor::DirMonitor(const std::string& path, Logger& logger) :m_inotify_fd(inotify_init()),
                                                m_dispatcher(),
                                                m_log(logger),
                                                m_thread(&DirMonitor::RunThread, this, path),
                                                m_is_running(true) 
{}





void DirMonitor::RunThread(const std::string& path)
{
    if (m_inotify_fd == -1)
    {
        std::string error_message("inotify_init: ");
        error_message += strerror(errno);
        m_log.Write(error_message, Logger::ERROR, 11, "dirmonitor.cpp");
        m_is_running = false;
        return;
    }
    int watch_desc = inotify_add_watch(m_inotify_fd, path.c_str(), IN_CREATE | IN_MODIFY);
    if (watch_desc == -1)
    {
        std::string error_message("inotify_add_watch: ");
        error_message += strerror(errno);
        m_log.Write(error_message, Logger::ERROR, 32, "dirmonitor.cpp");
        return;
    }

    m_log.Write("Start monitoring!", Logger::INFO, 41, "dirmonitor.cpp");

    fd_set set;
    struct timeval time_to_wait = {0};
    FD_ZERO(&set);
    int res = 0;
    struct inotify_event event;

    while (m_is_running == true)
    {
        FD_SET(m_inotify_fd, &set);
        time_to_wait.tv_sec = 7;
        res = select(m_inotify_fd + 1, &set, NULL, NULL, &time_to_wait);
        if (FD_ISSET(m_inotify_fd, &set))
        {
            if (read(m_inotify_fd, &event, 1024) < 0)
            {
                std::string error_message("read: ");
                error_message += strerror(errno);
                m_log.Write(error_message, Logger::ERROR, 53, "dirmonitor.cpp");
                continue;
            }
            m_dispatcher.NotifyAll(event.name);
        }
    }
    inotify_rm_watch(m_inotify_fd, watch_desc);
    close(m_inotify_fd);
    m_log.Write("Stop monitoring!", Logger::INFO, 68, "dirmonitor.cpp");
}



DirMonitor::~DirMonitor()
{
    m_is_running = false;
    m_thread.join();
}

Dispatcher<std::string> *DirMonitor::GetDispatcher()
{
    return &m_dispatcher;
}









