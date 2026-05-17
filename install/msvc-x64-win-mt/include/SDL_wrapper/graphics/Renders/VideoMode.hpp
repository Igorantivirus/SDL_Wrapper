#pragma once

#include <SDL_wrapper/Graphics/Export.hpp>

#include <SDL3/SDL_video.h>

namespace sdl3
{

struct SDL_WRAPPER_GRAPHICS_EXPORT VideoMode
{
public:
    int width{};
    int height{};

    bool resizable = true;
    bool borderless = false;
    bool fullscreen = false;
    bool highDpi = false;
    bool vsync = true;

public:
    static VideoMode getDefaultVideoMode();

    static SDL_WindowFlags makeWindowFlags(const VideoMode &settings);

};

} // namespace sdl3
