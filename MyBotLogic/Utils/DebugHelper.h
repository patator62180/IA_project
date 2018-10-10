#ifndef DEBUG_HELPER_H
#define DEBUG_HELPER_H

#include "../Utils/Singleton.h"
#include "MyBotLogic/MyBotLogic.h"

#include <functional>

#include "Logger.h"

class DebugHelper : Singleton
{
private:
    DebugHelper() = default;

    Logger logger;
public:
    //void test(std::function<void()> f) {
    //    try
    //    {
    //        f();
    //    }
    //    catch (const std::exception &exc)
    //    {
    //        BOT_LOGIC_LOG(logger, exc.what(), true);
    //    }
    //};

    void Log(const std::string& message) noexcept;
    void InitLogger(const std::string);

    static DebugHelper& getInstance() noexcept;
    ~DebugHelper() = default;
};

#endif // DEBUG_HELPER_H