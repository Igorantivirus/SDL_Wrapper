#pragma once

#include <cstddef>
#include <memory>
#include <string_view>

#include <SDL3/SDL_render.h>

#include <SDLWrapper/Renders/RenderTarget.hpp>
#include <SDLWrapper/Renders/VideoMode.hpp>

namespace sdl3
{

class RenderWindow : public RenderTarget
{
public:
    RenderWindow() = default;
    RenderWindow(std::string_view name, const VideoMode &mode);
    ~RenderWindow();

    bool create(std::string_view name, const VideoMode &mode);
    void close();

    std::size_t getWindowID() const;

    bool isOpen();
    bool isFullScreen() const;
    void setFullScreen(bool isFull);

    bool loadIconFromFile(std::string_view iconFileName);

private:
    
    bool isOpen_ = false;
    bool isFullScreen_ = false;

    std::shared_ptr<SDL_Window> window_;

    std::size_t windowID_ = std::size_t(-1);

private:
    void subscribe();
    void unsubscribe();
};

} // namespace sdl3

