#pragma once

#include <SDL3/SDL_rect.h>

namespace sdl3::operators
{

SDL_FPoint operator+(const SDL_FPoint &lhs, const SDL_FPoint &rhs);
SDL_FPoint &operator+=(SDL_FPoint &lhs, const SDL_FPoint &rhs);
SDL_FPoint operator-(const SDL_FPoint &lhs, const SDL_FPoint &rhs);
SDL_FPoint &operator-=(SDL_FPoint &lhs, const SDL_FPoint &rhs);
SDL_FPoint operator-(const SDL_FPoint &p);
SDL_FPoint operator*(const SDL_FPoint &p, float scalar);
SDL_FPoint operator*(float scalar, const SDL_FPoint &p);
SDL_FPoint &operator*=(SDL_FPoint &p, float scalar);
SDL_FPoint operator/(const SDL_FPoint &p, float scalar);
SDL_FPoint &operator/=(SDL_FPoint &p, float scalar);
bool operator==(const SDL_FPoint &lhs, const SDL_FPoint &rhs);
bool operator!=(const SDL_FPoint &lhs, const SDL_FPoint &rhs);

SDL_Point operator+(const SDL_Point &lhs, const SDL_Point &rhs);
SDL_Point operator-(const SDL_Point &lhs, const SDL_Point &rhs);
bool operator==(const SDL_Point &lhs, const SDL_Point &rhs);
bool operator!=(const SDL_Point &lhs, const SDL_Point &rhs);

SDL_FRect operator+(const SDL_FRect &rect, const SDL_FPoint &p);
SDL_FRect &operator+=(SDL_FRect &rect, const SDL_FPoint &p);
SDL_FRect operator-(const SDL_FRect &rect, const SDL_FPoint &p);
SDL_FRect &operator-=(SDL_FRect &rect, const SDL_FPoint &p);

SDL_Rect operator+(const SDL_Rect &rect, const SDL_Point &p);
SDL_Rect &operator+=(SDL_Rect &rect, const SDL_Point &p);
SDL_Rect operator-(const SDL_Rect &rect, const SDL_Point &p);
SDL_Rect &operator-=(SDL_Rect &rect, const SDL_Point &p);

bool operator==(const SDL_FRect &lhs, const SDL_FRect &rhs);
bool operator!=(const SDL_FRect &lhs, const SDL_FRect &rhs);
bool operator==(const SDL_Rect &lhs, const SDL_Rect &rhs);
bool operator!=(const SDL_Rect &lhs, const SDL_Rect &rhs);

} // namespace sdl3::operators