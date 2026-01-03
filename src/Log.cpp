#include <SDLWrapper/Log.hpp>

namespace sdl3::log
{

namespace config
{
bool toFile = false;
bool toConsole = true;
bool colorize = true;
bool showTime = true;
std::string logFileName = "latest_log.txt";

std::array<bool, (size_t)SDL_LogPriority::SDL_LOG_PRIORITY_COUNT> printLocation = {
    true,  // INVALID
    false, // TRACE
    false, // VERBOSE
    false, // DEBUG
    false, // INFO
    true,  // WARN
    true,  // ERROR
    true,  // CRITICAL
};

std::array<std::string_view, (size_t)SDL_LogPriority::SDL_LOG_PRIORITY_COUNT> colors = {
    "\033[0m",  // Invalid
    "\033[0m",  // Trace
    "\033[90m", // Verbose
    "\033[34m", // Debug
    "\033[32m", // Info
    "\033[33m", // Warn
    "\033[31m", // Error
    "\033[35m", // Critical
};
} // namespace config

std::string getTimeString()
{
    auto now = std::chrono::system_clock::now();
    return std::format("{:%H:%M:%S}", now);
}

std::string_view getLevelName(const SDL_LogPriority level)
{
    switch (level)
    {
    case SDL_LogPriority::SDL_LOG_PRIORITY_INVALID:
        return "INVALID";
    case SDL_LogPriority::SDL_LOG_PRIORITY_TRACE:
        return "TRACE";
    case SDL_LogPriority::SDL_LOG_PRIORITY_VERBOSE:
        return "VERBOSE";
    case SDL_LogPriority::SDL_LOG_PRIORITY_DEBUG:
        return "DEBUG";
    case SDL_LogPriority::SDL_LOG_PRIORITY_INFO:
        return "INFO";
    case SDL_LogPriority::SDL_LOG_PRIORITY_WARN:
        return "WARN";
    case SDL_LogPriority::SDL_LOG_PRIORITY_ERROR:
        return "ERROR";
    case SDL_LogPriority::SDL_LOG_PRIORITY_CRITICAL:
        return "CRITICAL";
    default:
        return "UNKNOWN";
    }
}

} // namespace sdl3::log

