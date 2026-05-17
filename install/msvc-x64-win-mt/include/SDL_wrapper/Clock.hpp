#pragma once

#include <cstdint>

namespace sdl3
{
class Clock
{
public:
    Clock();

    void start();
    void pause(bool pause);

    bool isPaused() const;

    std::uint64_t elapsedTimeMS() const;
    float elapsedTimeS() const;

private:
    std::uint64_t lastTimeMS_ = 0;
    std::uint64_t pauseStartMS_ = 0;
    bool paused_ = false;
};

class ClockNS
{
public:
    ClockNS();

    void start();
    void pause(bool pause);

    bool isPaused() const;

    std::uint64_t elapsedTimeNS() const;
    float elapsedTimeMS() const;
    float elapsedTimeS() const;

private:
    std::uint64_t lastTimeNS_;
    std::uint64_t pauseStartNS_ = 0;
    bool paused_ = false;
};

} // namespace sdl3