#ifndef DEBUG_HELPER_H
#define DEBUG_HELPER_H

#include "../Utils/Singleton.h"
#include "MyBotLogic/MyBotLogic.h"

#include <functional>
#include <chrono>


#include "Logger.h"

class DebugHelper : Singleton
{
private:
    DebugHelper() = default;

    Logger logger;
public:
    //template <class F, class ...Args>
    //auto chrono(F f, Args &&... args) {
    //    auto pre = high_resolution_clock::now();
    //    res = f(std::forward<Args>(args)...)
    //    auto post = high_resolution_clock::now();
    //    return pair{ res, post - pre; }
    //}

    void Log(const std::string& message) noexcept;
    void InitLogger(const std::string);

    static DebugHelper& getInstance() noexcept;
    ~DebugHelper() = default;
};

#endif // DEBUG_HELPER_H