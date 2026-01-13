#pragma once

#include <cstdint>

namespace sdl3
{
class Clock
{
public:
    Clock();

    void start();

    std::uint64_t elapsedTimeMS() const;
    float elapsedTimeS() const;

private:
    std::uint64_t lastTimeMS_;
};

class ClockNS
{
public:
    ClockNS();

    void start();

    std::uint64_t elapsedTimeNS() const;
    float elapsedTimeMS() const;
    float elapsedTimeS() const;

private:
    std::uint64_t lastTimeNS_;
};

} // namespace sdl3