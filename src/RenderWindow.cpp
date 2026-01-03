#include <SDLWrapper/Renders/RenderWindow.hpp>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>

#include <SDLWrapper/Log.hpp>
#include <SDLWrapper/Renders/RenderMeneger.hpp>

struct Deleterer
{
    void operator()(SDL_Window *window) const noexcept
    {
        SDL_DestroyWindow(window);
    }
    void operator()(SDL_Renderer *renderer) const noexcept
    {
        SDL_DestroyRenderer(renderer);
    }
};

namespace sdl3
{

RenderWindow::RenderWindow(const std::string_view name, const VideoMode &mode)
{
    void(create(name, mode));
}

RenderWindow::~RenderWindow()
{
    close();
}

bool RenderWindow::create(const std::string_view name, const VideoMode &mode)
{
    close();
    SDL_Window *wnd = nullptr;
    SDL_Renderer *rnd = nullptr;
    bool res = SDL_CreateWindowAndRenderer(name.data(), mode.width, mode.height, VideoMode::makeWindowFlags(mode), &wnd, &rnd);
    if (!res)
    {
        log::Error("{}", std::source_location::current(), SDL_GetError());
        return false;
    }

    window_.reset(wnd, Deleterer{});
    renderer_.reset(rnd, Deleterer{});

    subscribe();
    isFullScreen_ = mode.fullscreen;
    targetSize_.x = mode.width;
    targetSize_.y = mode.height;
    view_.setCenterPosition({mode.width / 2.f, mode.height / 2.f});
    isOpen_ = window_ && renderer_;
    return isOpen_;
}

void RenderWindow::close()
{
    window_.reset();
    renderer_.reset();
    view_.reset();
    unsubscribe();
    isOpen_ = isFullScreen_ = false;
}

std::size_t RenderWindow::getWindowID() const
{
    return windowID_;
}

bool RenderWindow::isOpen()
{
    return isOpen_;
}

bool RenderWindow::isFullScreen() const
{
    return isFullScreen_;
}

void RenderWindow::setFullScreen(const bool isFull)
{
    isFullScreen_ = isFull;
    SDL_SetWindowFullscreen(window_.get(), isFull);
}

bool RenderWindow::loadIconFromFile(const std::string_view iconFileName)
{
    if (!isOpen_)
        return false;
    SDL_Surface *icon = IMG_Load(iconFileName.data());

    if (!icon)
    {
        log::Error("{}", std::source_location::current(), SDL_GetError());
        return false;
    }
    bool res = SDL_SetWindowIcon(window_.get(), icon);
    SDL_DestroySurface(icon);
    return res;
}

void RenderWindow::subscribe()
{
    windowID_ = RenderMeneger::subscribeRenderer(renderer_);
}

void RenderWindow::unsubscribe()
{
    RenderMeneger::unsubscribeRenderer(windowID_);
    windowID_ = std::size_t(-1);
}

} // namespace sdl3
