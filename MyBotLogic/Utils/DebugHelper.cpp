#include "DebugHelper.h"

DebugHelper& DebugHelper::getInstance() noexcept {
    static DebugHelper instance;
    return instance;
}

void DebugHelper::InitLogger(std::string path) {
    logger.Init(path, "debug.log");
}

void DebugHelper::Log(const std::string& message) noexcept
{
    BOT_LOGIC_LOG(logger, message, true);
};
