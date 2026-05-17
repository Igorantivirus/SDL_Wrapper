#pragma once

#include <cstdint>

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_timer.h>

namespace sdl3::EventRegistrator
{

template <typename T>
concept PtrSizeable = sizeof(T) <= sizeof(void*);
template <typename T>
concept Int32Sizeable = sizeof(T) == sizeof(int);

template <Int32Sizeable T, PtrSizeable T1, PtrSizeable T2>
bool createEventCorrect(T eventType, int code, T1 data1 = nullptr, T2 data2 = nullptr)
{
    if (static_cast<int>(eventType) < SDL_EVENT_USER)
    {
        SDL_SetError("Event type must be >= SDL_EVENT_USER");
        return false;
    }

    SDL_Event event;
    SDL_zero(event);

    event.type = static_cast<int>(eventType);
    event.user.timestamp = SDL_GetTicks();
    event.user.code = code;
    event.user.data1 = reinterpret_cast<void *>(data1);
    event.user.data2 = reinterpret_cast<void *>(data2);

    SDL_PushEvent(&event);
    return true;
}

template <PtrSizeable T>
T ptr_cast(void* ptr)
{
    return static_cast<T>(reinterpret_cast<std::uintptr_t>(ptr));
}

} // namespace sdl3::EventRegistrator