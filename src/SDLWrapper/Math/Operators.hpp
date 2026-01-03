#pragma once

#include <SDL3/SDL_rect.h>

namespace sdl3::operators
{

inline SDL_FPoint operator+(const SDL_FPoint &lhs, const SDL_FPoint &rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}

inline SDL_FPoint &operator+=(SDL_FPoint &lhs, const SDL_FPoint &rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

inline SDL_FPoint operator-(const SDL_FPoint &lhs, const SDL_FPoint &rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

inline SDL_FPoint &operator-=(SDL_FPoint &lhs, const SDL_FPoint &rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

inline SDL_FPoint operator-(const SDL_FPoint &p)
{
    return {-p.x, -p.y};
}

inline SDL_FPoint operator*(const SDL_FPoint &p, float scalar)
{
    return {p.x * scalar, p.y * scalar};
}

inline SDL_FPoint operator*(float scalar, const SDL_FPoint &p)
{
    return {p.x * scalar, p.y * scalar};
}

inline SDL_FPoint &operator*=(SDL_FPoint &p, float scalar)
{
    p.x *= scalar;
    p.y *= scalar;
    return p;
}

inline SDL_FPoint operator/(const SDL_FPoint &p, float scalar)
{
    return {p.x / scalar, p.y / scalar};
}

inline SDL_FPoint &operator/=(SDL_FPoint &p, float scalar)
{
    p.x /= scalar;
    p.y /= scalar;
    return p;
}

inline bool operator==(const SDL_FPoint &lhs, const SDL_FPoint &rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

inline bool operator!=(const SDL_FPoint &lhs, const SDL_FPoint &rhs)
{
    return !(lhs == rhs);
}

// --- ОПЕРАТОРЫ ДЛЯ SDL_Point (int) ---

inline SDL_Point operator+(const SDL_Point &lhs, const SDL_Point &rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}

inline SDL_Point operator-(const SDL_Point &lhs, const SDL_Point &rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

inline bool operator==(const SDL_Point &lhs, const SDL_Point &rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}
inline bool operator!=(const SDL_Point &lhs, const SDL_Point &rhs)
{
    return !(lhs == rhs);
}

// --- ОПЕРАТОРЫ ДЛЯ RECT + POINT (Смещение) ---

inline SDL_FRect operator+(const SDL_FRect &rect, const SDL_FPoint &p)
{
    return {rect.x + p.x, rect.y + p.y, rect.w, rect.h};
}

inline SDL_FRect &operator+=(SDL_FRect &rect, const SDL_FPoint &p)
{
    rect.x += p.x;
    rect.y += p.y;
    return rect;
}

inline SDL_FRect operator-(const SDL_FRect &rect, const SDL_FPoint &p)
{
    return {rect.x - p.x, rect.y - p.y, rect.w, rect.h};
}

inline SDL_FRect &operator-=(SDL_FRect &rect, const SDL_FPoint &p)
{
    rect.x -= p.x;
    rect.y -= p.y;
    return rect;
}

inline SDL_Rect operator+(const SDL_Rect &rect, const SDL_Point &p)
{
    return {rect.x + p.x, rect.y + p.y, rect.w, rect.h};
}

inline SDL_Rect &operator+=(SDL_Rect &rect, const SDL_Point &p)
{
    rect.x += p.x;
    rect.y += p.y;
    return rect;
}

inline SDL_Rect operator-(const SDL_Rect &rect, const SDL_Point &p)
{
    return {rect.x - p.x, rect.y - p.y, rect.w, rect.h};
}

inline SDL_Rect &operator-=(SDL_Rect &rect, const SDL_Point &p)
{
    rect.x -= p.x;
    rect.y -= p.y;
    return rect;
}

inline bool operator==(const SDL_FRect &lhs, const SDL_FRect &rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.h == rhs.h) && (lhs.w == rhs.w);
}
inline bool operator!=(const SDL_FRect &lhs, const SDL_FRect &rhs)
{
    return !(lhs == rhs);
}

inline bool operator==(const SDL_Rect &lhs, const SDL_Rect &rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.h == rhs.h) && (lhs.w == rhs.w);
}
inline bool operator!=(const SDL_Rect &lhs, const SDL_Rect &rhs)
{
    return !(lhs == rhs);
}

} // namespace sdl3::operators