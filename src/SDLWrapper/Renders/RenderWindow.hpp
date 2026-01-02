#pragma once

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <memory>
#include <string_view>

#include <SDL3/SDL_video.h>

#include "RenderMeneger.hpp"
#include "RenderTarget.hpp"
#include "VideoMode.hpp"

namespace sdl3
{
class RenderWindow : public RenderTarget
{
public:
    RenderWindow() = default;
    RenderWindow(const std::string_view name, const VideoMode &mode)
    {
        void(create(name, mode));
    }
    ~RenderWindow()
    {
        close();
    }

    bool create(const std::string_view name, const VideoMode &mode)
    {
        close();
        SDL_Window *wnd = nullptr;
        SDL_Renderer *rnd = nullptr;
        bool res = SDL_CreateWindowAndRenderer(name.data(), mode.width, mode.height, VideoMode::makeWindowFlags(mode), &wnd, &rnd);
        if (!res)
        {
            SDL_Log("Error of SDL_CreateWindowAndRenderer in RenderWindow.hpp:28 with error: \"%s\"", SDL_GetError());
            return false;
        }
        window_.reset(wnd, SDL_DestroyWindow);
        renderer_.reset(rnd, SDL_DestroyRenderer);
        subscribe();
        isFullScreen_ = mode.fullscreen;
        targetSize_.x = mode.width;
        targetSize_.y = mode.height;
        RenderTarget::setBaseViewPosition({mode.width / 2.f, mode.height / 2.f});
        isOpen_ = window_ && renderer_;
        return isOpen_;
    }

    void close()
    {
        window_.reset();
        renderer_.reset();
        // view_.resetView();
        unsubscribe();
        isOpen_ = isFullScreen_ = false;
    }

    const std::size_t getWindowID() const
    {
        return windowID_;
    }

    bool isOpen()
    {
        return isOpen_;
    }
    bool isFullScreen() const
    {
        return isFullScreen_;
    }
    void setFullScreen(const bool isFull)
    {
        isFullScreen_ = isFull;
        SDL_SetWindowFullscreen(window_.get(), isFull);
    }

    bool loadIconFromFile(const std::string_view iconFileName)
    {
        if(!isOpen_)
            return false;
        SDL_Surface *icon = IMG_Load(iconFileName.data()); // или SDL_LoadBMP("icon.bmp");

        if (!icon)
        {
            SDL_Log("Error load icon fom Window in RenderWindow.hpp:86, load file: \"%s\", with error: \"%s\"", iconFileName.data(), SDL_GetError());
            return false;
        }
        bool res = SDL_SetWindowIcon(window_.get(), icon);
        SDL_DestroySurface(icon);
        return res;
    }

private:
    bool isOpen_ = false;
    bool isFullScreen_ = false;

    std::shared_ptr<SDL_Window> window_;
    // std::shared_ptr<SDL_Renderer> RenderTarget::renderer_;
    // View RenderTarget::view_;

    std::size_t windowID_ = std::size_t(-1);

private:
    void subscribe()
    {
        windowID_ = RenderMeneger::subscribeRenderer(renderer_);
    }
    void unsubscribe()
    {
        RenderMeneger::unsubscribeRenderer(windowID_);
        windowID_ = std::size_t(-1);
    }
};
} // namespace sdl3