#pragma once

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <cmath>

namespace sdl3
{

inline SDL_FPoint toFPoint(const SDL_Point &p)
{
    return {static_cast<float>(p.x), static_cast<float>(p.y)};
}

inline SDL_Point toPoint(const SDL_FPoint &p)
{
    return {static_cast<int>(std::round(p.x)), static_cast<int>(std::round(p.y))};
}

inline SDL_Color toColor(const SDL_FColor &c)
{
    return SDL_Color{static_cast<Uint8>(c.r * 255.0f), static_cast<Uint8>(c.g * 255.0f), static_cast<Uint8>(c.b * 255.0f), static_cast<Uint8>(c.a * 255.0f)};
}
inline SDL_FColor toFColor(const SDL_Color &c)
{
    return {c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, c.a / 255.0f};
}
inline SDL_FColor toFColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
{
    return SDL_FColor{r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f};
}
inline SDL_FColor toFColor(uint32_t hex)
{
    return {
        ((hex >> 24) & 0xFF) / 255.0f,
        ((hex >> 16) & 0xFF) / 255.0f,
        ((hex >> 8) & 0xFF) / 255.0f,
        (hex & 0xFF) / 255.0f};
}
} // namespace sdl3