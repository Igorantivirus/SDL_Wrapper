#pragma once

#include <SDL3/SDL_rect.h>

namespace sdl3
{

float length(const SDL_FPoint &p);
float distance(const SDL_FPoint &p1, const SDL_FPoint &p2);
SDL_FPoint normalize(const SDL_FPoint &p);
float dot(const SDL_FPoint &p1, const SDL_FPoint &p2);

} // namespace sdl3

