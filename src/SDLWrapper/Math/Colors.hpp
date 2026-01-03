#pragma once

#include <SDL3/SDL_pixels.h>

namespace sdl3::Color
{
inline static constexpr const SDL_FColor White = {1.0f, 1.0f, 1.0f, 1.0f};
inline static constexpr const SDL_FColor Black = {0.0f, 0.0f, 0.0f, 1.0f};
inline static constexpr const SDL_FColor Transparent = {0.0f, 0.0f, 0.0f, 0.0f};

inline static constexpr const SDL_FColor Red = {1.0f, 0.0f, 0.0f, 1.0f};
inline static constexpr const SDL_FColor Green = {0.0f, 1.0f, 0.0f, 1.0f};
inline static constexpr const SDL_FColor Blue = {0.0f, 0.0f, 1.0f, 1.0f};

inline static constexpr const SDL_FColor Yellow = {1.0f, 1.0f, 0.0f, 1.0f};
inline static constexpr const SDL_FColor Cyan = {0.0f, 1.0f, 1.0f, 1.0f};
inline static constexpr const SDL_FColor Magenta = {1.0f, 0.0f, 1.0f, 1.0f};

inline static constexpr const SDL_FColor Grey = {0.5f, 0.5f, 0.5f, 1.0f};
inline static constexpr const SDL_FColor DarkGrey = {0.25f, 0.25f, 0.25f, 1.0f};
inline static constexpr const SDL_FColor LightGrey = {0.75f, 0.75f, 0.75f, 1.0f};
} // namespace sdl3::Color