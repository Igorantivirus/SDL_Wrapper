#include <SDLWrapper/Math/Convert.hpp>

#include <cmath>

namespace sdl3
{

SDL_FPoint toFPoint(const SDL_Point &p)
{
    return {static_cast<float>(p.x), static_cast<float>(p.y)};
}

SDL_Point toPoint(const SDL_FPoint &p)
{
    return {static_cast<int>(std::round(p.x)), static_cast<int>(std::round(p.y))};
}

SDL_Color toColor(const SDL_FColor &c)
{
    return SDL_Color{
        static_cast<Uint8>(c.r * 255.0f),
        static_cast<Uint8>(c.g * 255.0f),
        static_cast<Uint8>(c.b * 255.0f),
        static_cast<Uint8>(c.a * 255.0f)};
}

SDL_FColor toFColor(const SDL_Color &c)
{
    return {c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, c.a / 255.0f};
}

SDL_FColor toFColor(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a)
{
    return SDL_FColor{r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f};
}

SDL_FColor toFColor(const std::uint32_t hex)
{
    return {
        ((hex >> 24) & 0xFF) / 255.0f,
        ((hex >> 16) & 0xFF) / 255.0f,
        ((hex >> 8) & 0xFF) / 255.0f,
        (hex & 0xFF) / 255.0f};
}

} // namespace sdl3

