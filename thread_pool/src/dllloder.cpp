#include <dlfcn.h>

#include "dllloader.hpp"

using namespace ilrd;


DllLoader::DllLoader(Dispatcher<std::string>* dispatcher, void **library_ptr) : m_callback(dispatcher, *this, &DllLoader::Action),
                                                                                m_library_ptr(library_ptr) {}


void DllLoader::Action(const std::string& lib)
{
    if (lib.compare(lib.length() - 3, 3, ".so") == 0)
    {
        *m_library_ptr = dlopen(lib.c_str(), RTLD_LAZY);
    }
}


