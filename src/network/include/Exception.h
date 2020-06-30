#pragma once

#include "common.h"
#define _S_LINE(x) #x
#define __S_LINE(x) _S_LINE(x)
#define __S_LINE__ __S_LINE(__LINE__)
#ifdef DEBUG_MeyaS
    #define DebugException(str)  throw Exception(__FILE__ ":" __S_LINE__ " Exception:" str)
#else
    #ifdef LOG_MeyaS
        #define DebugException(str) std::cerr<<__S_FILE__ ":" __S_LINE__ " Exception:" str<<std::endl
    #else
        #define DebugException(str)
    #endif
#endif
namespace MeyaS {
    class Exception : public std::exception {
    public:
        Exception();

        explicit Exception(std::string reason);

        const char *what() const noexcept override;

    private:
        std::string reason;
    };
}