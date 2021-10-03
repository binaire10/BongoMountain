#ifndef GAMEENGINEBONGO_ASSERT_HPP
#define GAMEENGINEBONGO_ASSERT_HPP
#include <cstdlib>
#include "Log.hpp"

#ifndef NDEBUG
#    if defined(WIN32)
#        define BM_DEBUGBREAK() __debugbreak()
#    elif defined(linux)
#        include <signal.h>
#        define BM_DEBUGBREAK() raise(SIGTRAP)
#    else
#        error unsupport platform
#    endif

#    define BM_CUSTOM_ASSERT(C, x, ...)                           \
        {                                                         \
            if(!(x))                                              \
            {                                                     \
                C("Assertion Failed (" #x "): " __VA_ARGS__); \
                BM_DEBUGBREAK();                                  \
                std::exit(1);                                     \
            }                                                     \
        }

#    define BM_ASSERT(x, ...)      BM_CUSTOM_ASSERT(BM_ERROR, x, __VA_ARGS__)
#    define BM_CORE_ASSERT(x, ...) BM_CUSTOM_ASSERT(BM_CORE_ERROR, x, __VA_ARGS__)
#else
#    define BM_ASSERT(x, ...)
#    define BM_CORE_ASSERT(x, ...)
#endif

#endif// GAMEENGINEBONGO_ASSERT_HPP
