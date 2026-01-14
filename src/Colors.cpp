#include <SDLWrapper/Math/Colors.hpp>

namespace sdl3
{

Color Color::toColor(const std::uint32_t hex)
{
    return {
        ((hex >> 24) & 0xFF) / 255.0f,
        ((hex >> 16) & 0xFF) / 255.0f,
        ((hex >> 8) & 0xFF) / 255.0f,
        (hex & 0xFF) / 255.0f};
}
Color Color::toColor(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a)
{
    return {r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f};
}
std::uint32_t Color::toHex(const Color &color)
{
    return (std::uint32_t(color.r * 255.0f) << 24) |
           (std::uint32_t(color.g * 255.0f) << 16) |
           (std::uint32_t(color.b * 255.0f) << 8) |
           std::uint32_t(color.a * 255.0f);
}

bool Color::operator==(const Color &color) const
{
    return r == color.r &&
           g == color.g &&
           b == color.b &&
           a == color.a;
}
bool Color::operator!=(const Color &color) const
{
    return !this->operator==(color);
}

} // namespace sdl3
