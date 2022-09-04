#include <fstream>
#include <iostream>
#include <ctime>
#include <chrono>

#include "logger.hpp"

using namespace ilrd;




Logger::Logger() : m_path("logger.txt"), m_level(INFO), m_file(), m_threadpool(1)
{
    m_file.open(m_path.c_str(), std::ios::app);
}


void Logger::SetPath(const std::string& path)
{
    m_file.close();
    remove(m_path.c_str());
    m_path = path;
    m_file.open(m_path.c_str(), std::ios::app);
}


void Logger::SetLevel(LogLevel level)
{
    m_level = level;
}


Logger::~Logger()
{
    m_threadpool.Stop();
    m_file.close();
}


Logger::LogLevel Logger::GetLevel() const
{   
    return m_level;
}


void Logger::Write(const std::string &message, LogLevel level, int line, const std::string& src)
{
    static std::string level_arr[] = {"[INFO]", "[ERROR]", "[DEBUG]"};
    if (level <= m_level)
    {
        auto time  = std::chrono::high_resolution_clock::now();
        std::time_t actual_time = std::chrono::system_clock::to_time_t(time);
        std::shared_ptr<Task> task = std::make_shared<WriteTask>(message, level_arr[level], line, std::this_thread::get_id(), actual_time, m_file, src);
        m_threadpool.AddTask(task);
    }
}



WriteTask::WriteTask(const std::string &message, const std::string &level, int line, const std::thread::id &thread_id, time_t time, std::ofstream &file, const std::string &file_name):
                        Task(static_cast<Priority>(HIGH + 4)),
                        m_message(message),
                        m_level(level),
                        m_line(line),
                        m_thread_id(thread_id),
                        m_time(time),
                        m_file(file),
                        m_file_name(file_name) {}


void WriteTask::Execute()
{
    char *dt = ctime(&m_time);
    m_file << dt << "TID: " << m_thread_id << ", Level: " << m_level << ", File: " << m_file_name << ", Line: " << m_line << ", Message: " << m_message << std::endl;
    m_file << std::endl;



}

