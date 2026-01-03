#include <SDLWrapper/Math/Operators.hpp>

namespace sdl3::operators
{

SDL_FPoint operator+(const SDL_FPoint &lhs, const SDL_FPoint &rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}

SDL_FPoint &operator+=(SDL_FPoint &lhs, const SDL_FPoint &rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

SDL_FPoint operator-(const SDL_FPoint &lhs, const SDL_FPoint &rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

SDL_FPoint &operator-=(SDL_FPoint &lhs, const SDL_FPoint &rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

SDL_FPoint operator-(const SDL_FPoint &p)
{
    return {-p.x, -p.y};
}

SDL_FPoint operator*(const SDL_FPoint &p, const float scalar)
{
    return {p.x * scalar, p.y * scalar};
}

SDL_FPoint operator*(const float scalar, const SDL_FPoint &p)
{
    return {p.x * scalar, p.y * scalar};
}

SDL_FPoint &operator*=(SDL_FPoint &p, const float scalar)
{
    p.x *= scalar;
    p.y *= scalar;
    return p;
}

SDL_FPoint operator/(const SDL_FPoint &p, const float scalar)
{
    return {p.x / scalar, p.y / scalar};
}

SDL_FPoint &operator/=(SDL_FPoint &p, const float scalar)
{
    p.x /= scalar;
    p.y /= scalar;
    return p;
}

bool operator==(const SDL_FPoint &lhs, const SDL_FPoint &rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

bool operator!=(const SDL_FPoint &lhs, const SDL_FPoint &rhs)
{
    return !(lhs == rhs);
}

SDL_Point operator+(const SDL_Point &lhs, const SDL_Point &rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}

SDL_Point operator-(const SDL_Point &lhs, const SDL_Point &rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

bool operator==(const SDL_Point &lhs, const SDL_Point &rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

bool operator!=(const SDL_Point &lhs, const SDL_Point &rhs)
{
    return !(lhs == rhs);
}

SDL_FRect operator+(const SDL_FRect &rect, const SDL_FPoint &p)
{
    return {rect.x + p.x, rect.y + p.y, rect.w, rect.h};
}

SDL_FRect &operator+=(SDL_FRect &rect, const SDL_FPoint &p)
{
    rect.x += p.x;
    rect.y += p.y;
    return rect;
}

SDL_FRect operator-(const SDL_FRect &rect, const SDL_FPoint &p)
{
    return {rect.x - p.x, rect.y - p.y, rect.w, rect.h};
}

SDL_FRect &operator-=(SDL_FRect &rect, const SDL_FPoint &p)
{
    rect.x -= p.x;
    rect.y -= p.y;
    return rect;
}

SDL_Rect operator+(const SDL_Rect &rect, const SDL_Point &p)
{
    return {rect.x + p.x, rect.y + p.y, rect.w, rect.h};
}

SDL_Rect &operator+=(SDL_Rect &rect, const SDL_Point &p)
{
    rect.x += p.x;
    rect.y += p.y;
    return rect;
}

SDL_Rect operator-(const SDL_Rect &rect, const SDL_Point &p)
{
    return {rect.x - p.x, rect.y - p.y, rect.w, rect.h};
}

SDL_Rect &operator-=(SDL_Rect &rect, const SDL_Point &p)
{
    rect.x -= p.x;
    rect.y -= p.y;
    return rect;
}

bool operator==(const SDL_FRect &lhs, const SDL_FRect &rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.h == rhs.h) && (lhs.w == rhs.w);
}

bool operator!=(const SDL_FRect &lhs, const SDL_FRect &rhs)
{
    return !(lhs == rhs);
}

bool operator==(const SDL_Rect &lhs, const SDL_Rect &rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.h == rhs.h) && (lhs.w == rhs.w);
}

bool operator!=(const SDL_Rect &lhs, const SDL_Rect &rhs)
{
    return !(lhs == rhs);
}

} // namespace sdl3::operators

