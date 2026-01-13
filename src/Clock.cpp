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
}
std::uint64_t Clock::elapsedTimeMS() const
{
    return SDL_GetTicks() - lastTimeMS_;
}
float Clock::elapsedTimeS() const
{
    return (SDL_GetTicks() - lastTimeMS_) / 1e3f;
}

ClockNS::ClockNS()
{
    start();
}
void ClockNS::start()
{
    lastTimeNS_ = SDL_GetTicksNS();
}
std::uint64_t ClockNS::elapsedTimeNS() const
{
    return SDL_GetTicksNS() - lastTimeNS_;
}
float ClockNS::elapsedTimeMS() const
{
    return (SDL_GetTicksNS() - lastTimeNS_) / 1e6f;
}
float ClockNS::elapsedTimeS() const
{
    return (SDL_GetTicksNS() - lastTimeNS_) / 1e9f;
}
} // namespace sdl3