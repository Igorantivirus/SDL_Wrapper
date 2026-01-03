#pragma once

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

namespace sdl3
{

struct Matrix3x3
{
    float a, b, c, d, tx, ty;

    Matrix3x3();

    SDL_FPoint transform(SDL_FPoint p) const;

    Matrix3x3 operator*(const Matrix3x3 &other) const;
};

} // namespace sdl3