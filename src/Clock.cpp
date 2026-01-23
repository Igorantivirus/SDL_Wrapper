#include <SDLWrapper/Clock.hpp>

#include <SDL3/SDL_timer.h>

namespace sdl3
{

Clock::Clock()
{
    start();
}
void Clock::start()
{
    lastTimeMS_ = SDL_GetTicks();
    paused_ = false;
    pauseStartMS_ = 0;
}
void Clock::pause(bool pause)
{
    if (pause == paused_)
        return;
    if (pause)
    {
        pauseStartMS_ = SDL_GetTicks();
    }
    else
    {
        const std::uint64_t elapsed = SDL_GetTicks() - pauseStartMS_;
        lastTimeMS_ += elapsed;
    }
    paused_ = pause;
}
bool Clock::isPaused() const
{
    return paused_;
}
std::uint64_t Clock::elapsedTimeMS() const
{
    if (paused_)
        return pauseStartMS_ - lastTimeMS_;
    return SDL_GetTicks() - lastTimeMS_;
}
float Clock::elapsedTimeS() const
{
    return elapsedTimeMS() / 1e3f;
}

} // namespace sdl3

namespace sdl3
{

ClockNS::ClockNS()
{
    start();
}
void ClockNS::start()
{
    lastTimeNS_ = SDL_GetTicksNS();
    paused_ = false;
    pauseStartNS_ = 0;
}
void ClockNS::pause(bool pause)
{
    if (pause == paused_)
        return;
    if (pause)
    {
        pauseStartNS_ = SDL_GetTicksNS();
    }
    else
    {
        const std::uint64_t elapsed = SDL_GetTicksNS() - pauseStartNS_;
        lastTimeNS_ += elapsed;
    }
    paused_ = pause;
}
bool ClockNS::isPaused() const
{
    return paused_;
}
std::uint64_t ClockNS::elapsedTimeNS() const
{
    if (paused_)
        return pauseStartNS_ - lastTimeNS_;
    return SDL_GetTicksNS() - lastTimeNS_;
}
float ClockNS::elapsedTimeMS() const
{
    return elapsedTimeNS() / 1e6f;
}
float ClockNS::elapsedTimeS() const
{
    return elapsedTimeNS() / 1e9f;
}
} // namespace sdl3