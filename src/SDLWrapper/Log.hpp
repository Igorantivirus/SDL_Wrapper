#pragma once

#include <SDL3/SDL_log.h>

#include <array>
#include <chrono>
#include <format>
#include <fstream>
#include <iostream>
#include <source_location>
#include <string>
#include <string_view>

namespace sdl3::log
{
// --- НАСТРОЙКИ ---
namespace config
{
inline bool toFile = false;
inline bool toConsole = true;
inline bool colorize = true;
inline bool showTime = true;
inline std::string logFileName = "latest_log.txt";

// Настройка вывода локации для каждого уровня
inline std::array<bool, (size_t)SDL_LogPriority::SDL_LOG_PRIORITY_COUNT> printLocation = {
    true,  // INVALID
    false, // TRACE
    false, // VERBOSE
    false, // DEBUG
    false, // INFO
    true,  // WARN
    true,  // ERROR
    true,  // CRITICAL
};

// ANSI цвета для Desktop
inline std::array<std::string_view, (size_t)SDL_LogPriority::SDL_LOG_PRIORITY_COUNT> colors = {
    "\033[0m",  // Invalid (Сброс)
    "\033[0m",  // Trace (Сброс)
    "\033[90m", // Verbose (Серый)
    "\033[34m", // Debug (Синий)
    "\033[32m", // Info (Зеленый)
    "\033[33m", // Warn (Желтый)
    "\033[31m", // Error (Красный)
    "\033[35m", // Critical (Пурпурный)
};
} // namespace config

// Вспомогательная функция для получения времени
inline std::string getTimeString()
{
    auto now = std::chrono::system_clock::now();
    return std::format("{:%H:%M:%S}", now);
}

// Вспомогательная функция для имени уровня
inline std::string_view getLevelName(const SDL_LogPriority level)
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

// --- ГЛАВНАЯ ФУНКЦИЯ ЛОГИРОВАНИЯ ---
template <typename... Args>
void Log(const SDL_LogPriority level, const std::source_location &loc, const std::string_view fmt, Args &&...args)
{
    // Проверка: нужно ли SDL вообще обрабатывать этот лог
    // if (SDL_GetLogPriority(SDL_LOG_CATEGORY_APPLICATION) > (SDL_LogPriority)level)
    //     return;

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
        // оставляем как было: без [LEVEL], просто сообщение (но с временем, если включено)
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                       (SDL_LogPriority)level,
                       "%s%s",
                       timeStr.c_str(),
                       message.c_str());
    }

#else
    // Desktop логирование
    std::string finalOutput;
    std::string locationStr = "";
    std::string timeStr = config::showTime ? std::format("[{}]", getTimeString()) : "";

    if (config::printLocation[(size_t)level])
        locationStr = std::format("@ {}:{}", loc.file_name(), loc.line());

    // Сборка строки для консоли (с цветами)
    if (config::toConsole)
    {
        std::string color = config::colorize ? std::string(config::colors[(size_t)level]) : "";
        std::string reset = config::colorize ? "\033[0m" : "";

        SDL_Log("%s %s[%s]%s %s %s", timeStr.c_str(), color.c_str(), getLevelName(level).data(), reset.c_str(), message.c_str(), locationStr.c_str());
    }

    // Сборка строки для файла (без цветов)
    if (config::toFile)
    {
        std::ofstream file(config::logFileName, std::ios::app);
        if (file.is_open())
        {
            file << timeStr << " [" << getLevelName(level) << "] " << message << ' ' << locationStr << "\n";
        }
    }
#endif
}

// --- ОБЕРТКИ ДЛЯ УРОВНЕЙ ---
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

// --- МАКРОСЫ ---
#ifndef SDL3_LOG_MACROS_DISABLE
#define LOG_VERBOSE(fmt, ...) ::sdl3::log::Verbose(fmt, std::source_location::current(), ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) ::sdl3::log::Debug(fmt, std::source_location::current(), ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) ::sdl3::log::Info(fmt, std::source_location::current(), ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) ::sdl3::log::Warn(fmt, std::source_location::current(), ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) ::sdl3::log::Error(fmt, std::source_location::current(), ##__VA_ARGS__)
#define LOG_CRITICAL(fmt, ...) ::sdl3::log::Critical(fmt, std::source_location::current(), ##__VA_ARGS__)
#endif