#pragma once

#include <SDL3/SDL_video.h>

namespace sdl3
{

struct VideoMode
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

// private:
//     static void initDefaultFlags(VideoMode &settings);
//     static void initDefaultSize(VideoMode &settings);
};

} // namespace sdl3
