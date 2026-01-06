#include <SDL3/SDL_error.h>
#include <SDLWrapper/Renders/VideoMode.hpp>

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_Log.h>

namespace sdl3
{

void initDefaultFlags(VideoMode &settings)
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

void initDefaultSize(VideoMode &settings)
{
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
    {
        SDL_Log("%s", SDL_GetError());
        return;
    }
    SDL_DisplayID displayID = SDL_GetPrimaryDisplay();
    const SDL_DisplayMode *mode = SDL_GetCurrentDisplayMode(displayID);
    if (!mode)
    {
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        SDL_Log("%s", SDL_GetError());
        return;
    }

    settings.width = static_cast<unsigned int>(mode->w);
    settings.height = static_cast<unsigned int>(mode->h);

    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

VideoMode VideoMode::getDefaultVideoMode()
{
    VideoMode res;

    initDefaultSize(res);
    initDefaultFlags(res);

    return res;
}

SDL_WindowFlags VideoMode::makeWindowFlags(const VideoMode &settings)
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

} // namespace sdl3
