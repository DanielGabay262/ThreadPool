#ifndef __ILRD_RD1167_LOGGER_HPP__
#define __ILRD_RD1167_LOGGER_HPP__

#include <mutex>
#include <thread>  //thread
#include <fstream>

#include "singleton.hpp"
#include "threadpool.hpp"


namespace ilrd
{

class Logger
{
public:
    enum LogLevel{INFO, ERROR, DEBUG};
    Logger(const Logger& other) = delete;
    Logger& operator=(const Logger& other) = delete;
    ~Logger();
    void SetPath(const std::string& path);
    void SetLevel(LogLevel level);
    LogLevel GetLevel() const;
    void Write(const std::string &message, LogLevel level, int line, const std::string& src);

private:
    Logger();
    friend class Singleton<Logger>;
    friend class WriteTask;
    std::string m_path;
    LogLevel m_level;
    std::ofstream m_file;
    ThreadPool m_threadpool;

};


class WriteTask : public Task
{
public:
    WriteTask(const std::string &message, const std::string &level, int line, const std::thread::id &thread_id, time_t time, std::ofstream &file, const std::string &file_name);
private:
    void Execute();
    std::string m_message;
    std::string m_level;
    int m_line;
    std::thread::id m_thread_id;
    std::time_t m_time;
    std::ofstream &m_file;
    std::string m_file_name;
};

}//ilrd



#endif /* __ILRD_RD1167_LOGGER_HPP__*/