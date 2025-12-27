#pragma once

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
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
    static VideoMode getDefaultVideoMode()
    {
        VideoMode res;

        initDefaultSize(res);
        initDefaultFlags(res);

        return res;
    }

    static SDL_WindowFlags makeWindowFlags(const VideoMode &settings)
    {

        SDL_WindowFlags flags = 0;
        if (settings.resizable)
            flags = static_cast<SDL_WindowFlags>(flags | SDL_WINDOW_RESIZABLE);
        if (settings.borderless)
            flags = static_cast<SDL_WindowFlags>(flags | SDL_WINDOW_BORDERLESS);
        if (settings.fullscreen)
            flags = static_cast<SDL_WindowFlags>(flags | SDL_WINDOW_FULLSCREEN);
        if (settings.highDpi)
            flags = static_cast<SDL_WindowFlags>(flags | SDL_WINDOW_HIGH_PIXEL_DENSITY);
        return flags;
    }

private:
    static void initDefaultFlags(VideoMode &settings)
    {
#ifdef ANDROID
        settings.resizable = true;
        settings.borderless = true;
        settings.fullscreen = true;
        settings.highDpi = false;
        settings.vsync = true;
#else
        settings.resizable = true;
        settings.borderless = false;
        settings.fullscreen = false;
        settings.highDpi = false;
        settings.vsync = true;
#endif
    }
    static void initDefaultSize(VideoMode &settings)
    {
        if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
        {
            SDL_Log("Error of SDL_InitSubSystem in WindowSettings.hpp:21 \"%s\"\n", SDL_GetError());
            return;
        }
        SDL_DisplayID displayID = SDL_GetPrimaryDisplay();
        const SDL_DisplayMode *mode = SDL_GetCurrentDisplayMode(displayID);
        if (!mode)
        {
            SDL_QuitSubSystem(SDL_INIT_VIDEO);
            SDL_Log("Error of SDL_GetCurrentDisplayMode in WindowSettings.hpp:30 \"%s\"\n", SDL_GetError());
            return;
        }

        settings.width = static_cast<unsigned int>(mode->w);
        settings.height = static_cast<unsigned int>(mode->h);
        // settings.refresh_rate = static_cast<unsigned int>(mode->refresh_rate);

        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }
};

} // namespace sdl3