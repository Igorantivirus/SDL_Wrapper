#pragma once

#include <SDL3/SDL_rect.h>
#include <cmath>

namespace sdl3
{
inline float length(const SDL_FPoint &p)
{
    return std::sqrt(p.x * p.x + p.y * p.y);
}

inline float distance(const SDL_FPoint &p1, const SDL_FPoint &p2)
{
    return length({p1.x - p2.x, p1.y - p2.y});
}

inline SDL_FPoint normalize(const SDL_FPoint &p)
{
    float len = length(p);
    if (len < 0.0001f)
        return {0, 0};
    return {p.x / len, p.y / len};
}

inline float dot(const SDL_FPoint &p1, const SDL_FPoint &p2)
{
    return p1.x * p2.x + p1.y * p2.y;
}
} // namespace sdl3::math