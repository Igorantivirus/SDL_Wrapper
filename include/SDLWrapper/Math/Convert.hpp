#pragma once

#include <cstdint>

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>

namespace sdl3
{

SDL_FPoint toFPoint(const SDL_Point &p);
SDL_Point toPoint(const SDL_FPoint &p);

SDL_Color toColor(const SDL_FColor &c);
SDL_FColor toFColor(const SDL_Color &c);
SDL_FColor toFColor(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255);
SDL_FColor toFColor(std::uint32_t hex);

} // namespace sdl3

