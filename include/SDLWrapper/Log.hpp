#pragma once

#include <SDL3/SDL_log.h>

#include <array>
#include <chrono>
#include <format>
#include <fstream>
#include <source_location>
#include <string>
#include <string_view>
#include <utility>

namespace sdl3::log
{

namespace config
{
extern bool toFile;
extern bool toConsole;
extern bool colorize;
extern bool showTime;
extern std::string logFileName;

extern std::array<bool, (size_t)SDL_LogPriority::SDL_LOG_PRIORITY_COUNT> printLocation;
extern std::array<std::string_view, (size_t)SDL_LogPriority::SDL_LOG_PRIORITY_COUNT> colors;
} // namespace config

std::string getTimeString();
std::string_view getLevelName(SDL_LogPriority level);

template <typename... Args>
void Log(const SDL_LogPriority level, const std::source_location &loc, const std::string_view fmt, Args &&...args)
{
    std::string message;
    try
    {
        message = std::vformat(fmt, std::make_format_args(args...));
    }
    catch (...)
    {
        message = "Formatting Error in log!";
    }

#ifdef __ANDROID__
    if (!config::toConsole)
        return;

    std::string timeStr = config::showTime ? std::format("[{}] ", getTimeString()) : "";

    if (config::printLocation[(size_t)level])
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                       (SDL_LogPriority)level,
                       "%s[%s] %s (%s:%u)",
                       timeStr.c_str(),
                       getLevelName(level).data(),
                       message.c_str(),
                       loc.file_name(),
                       (unsigned)loc.line());
    }
    else
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                       (SDL_LogPriority)level,
                       "%s%s",
                       timeStr.c_str(),
                       message.c_str());
    }
#else
    std::string locationStr = "";
    std::string timeStr = config::showTime ? std::format("[{}]", getTimeString()) : "";

    if (config::printLocation[(size_t)level])
        locationStr = std::format("@ {}:{}", loc.file_name(), loc.line());

    if (config::toConsole)
    {
        std::string color = config::colorize ? std::string(config::colors[(size_t)level]) : "";
        std::string reset = config::colorize ? "\033[0m" : "";

        SDL_Log("%s %s[%s]%s %s %s",
                timeStr.c_str(),
                color.c_str(),
                getLevelName(level).data(),
                reset.c_str(),
                message.c_str(),
                locationStr.c_str());
    }

    if (config::toFile)
    {
        std::ofstream file(config::logFileName, std::ios::app);
        if (file.is_open())
            file << timeStr << " [" << getLevelName(level) << "] " << message << ' ' << locationStr << "\n";
    }
#endif
}

template <typename... Args>
void Verbose(std::string_view fmt, const std::source_location loc, Args &&...args)
{
    Log(SDL_LogPriority::SDL_LOG_PRIORITY_VERBOSE, loc, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void Debug(std::string_view fmt, const std::source_location loc, Args &&...args)
{
    Log(SDL_LogPriority::SDL_LOG_PRIORITY_DEBUG, loc, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void Info(std::string_view fmt, const std::source_location loc, Args &&...args)
{
    Log(SDL_LogPriority::SDL_LOG_PRIORITY_INFO, loc, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void Warn(std::string_view fmt, const std::source_location loc, Args &&...args)
{
    Log(SDL_LogPriority::SDL_LOG_PRIORITY_WARN, loc, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void Error(std::string_view fmt, const std::source_location loc, Args &&...args)
{
    Log(SDL_LogPriority::SDL_LOG_PRIORITY_ERROR, loc, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void Critical(std::string_view fmt, const std::source_location loc, Args &&...args)
{
    Log(SDL_LogPriority::SDL_LOG_PRIORITY_CRITICAL, loc, fmt, std::forward<Args>(args)...);
}

} // namespace sdl3::log

#ifndef SDL3_LOG_MACROS_DISABLE
#define LOG_VERBOSE(fmt, ...) ::sdl3::log::Verbose(fmt, std::source_location::current(), ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) ::sdl3::log::Debug(fmt, std::source_location::current(), ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) ::sdl3::log::Info(fmt, std::source_location::current(), ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) ::sdl3::log::Warn(fmt, std::source_location::current(), ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) ::sdl3::log::Error(fmt, std::source_location::current(), ##__VA_ARGS__)
#define LOG_CRITICAL(fmt, ...) ::sdl3::log::Critical(fmt, std::source_location::current(), ##__VA_ARGS__)
#endif
