#ifndef __ILRD_RD1167_DLLLOADER_HPP__
#define __ILRD_RD1167_DLLLOADER_HPP__

#include "dispatcher.hpp"

namespace ilrd
{


class DllLoader
{
public:
    DllLoader(Dispatcher<std::string>* dispatcher, void **library_ptr) noexcept; 
    ~DllLoader() = default;
    DllLoader(const DllLoader& other) = delete;
    DllLoader& operator=(const DllLoader& other) = delete;
    void Action(const std::string& lib);
private:
    Callback<std::string, DllLoader> m_callback;
    void **m_library_ptr;
};

} // ilrd


#endif //__ILRD_RD1167_DLLLOADER_HPP__