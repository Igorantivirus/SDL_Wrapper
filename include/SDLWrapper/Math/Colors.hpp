#pragma once

#include <cstdint>

namespace sdl3
{
struct Color
{
    float r{};
    float g{};
    float b{};
    float a = 1.f;

    static Color toColor(const std::uint32_t hex);
    static Color toColor(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a = 255);
    static std::uint32_t toHex(const Color &color);
};
} // namespace sdl3

namespace sdl3::Colors
{
inline static constexpr const Color White = {1.0f, 1.0f, 1.0f, 1.0f};
inline static constexpr const Color Black = {0.0f, 0.0f, 0.0f, 1.0f};
inline static constexpr const Color Transparent = {0.0f, 0.0f, 0.0f, 0.0f};

inline static constexpr const Color Red = {1.0f, 0.0f, 0.0f, 1.0f};
inline static constexpr const Color Green = {0.0f, 1.0f, 0.0f, 1.0f};
inline static constexpr const Color Blue = {0.0f, 0.0f, 1.0f, 1.0f};

inline static constexpr const Color Yellow = {1.0f, 1.0f, 0.0f, 1.0f};
inline static constexpr const Color Cyan = {0.0f, 1.0f, 1.0f, 1.0f};
inline static constexpr const Color Magenta = {1.0f, 0.0f, 1.0f, 1.0f};

inline static constexpr const Color Grey = {0.5f, 0.5f, 0.5f, 1.0f};
inline static constexpr const Color DarkGrey = {0.25f, 0.25f, 0.25f, 1.0f};
inline static constexpr const Color LightGrey = {0.75f, 0.75f, 0.75f, 1.0f};
} // namespace sdl3::Colors