#include <SDLWrapper/Renders/RenderWindow.hpp>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>

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
        SDL_Log("%s", SDL_GetError());
        return false;
    }

    window_.reset(wnd, Deleterer{});
    renderer_.reset(rnd, Deleterer{});

    subscribe();
    isFullScreen_ = mode.fullscreen;
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
        SDL_Log("%s", SDL_GetError());
        return false;
    }
    bool res = SDL_SetWindowIcon(window_.get(), icon);
    SDL_DestroySurface(icon);
    return res;
}

bool RenderWindow::setLogicalPresentation(const Vector2i &size)
{
    bool res = SDL_SetRenderLogicalPresentation(renderer_.get(), size.x, size.y, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    if (!res)
        SDL_Log("%s", SDL_GetError());
    return res;
}

float RenderWindow::getDisplayScale() const
{
    return SDL_GetWindowDisplayScale(const_cast<SDL_Window *>(window_.get()));
}

float RenderWindow::getPixelDensity() const
{
    const float density = SDL_GetWindowPixelDensity(window_.get());
    if (density == 0.0f)
        SDL_Log("%s", SDL_GetError());
    return density;
}

void RenderWindow::subscribe()
{
    windowID_ = RenderMeneger::subscribeRenderer(renderer_);
}

void RenderWindow::unsubscribe()
{
    RenderMeneger::unsubscribeRenderer(windowID_);
    windowID_ = RenderMeneger::invalidID;
}

Vector2i RenderWindow::getSize() const
{
    Vector2i size{};
    if (!SDL_GetRenderOutputSize(renderer_.get(), &size.x, &size.y))
        SDL_Log("%s", SDL_GetError());
    return size;
}

Vector2i RenderWindow::getLogicSize() const
{
    Vector2i size{};
    SDL_RendererLogicalPresentation mode = SDL_LOGICAL_PRESENTATION_DISABLED;
    if (!SDL_GetRenderLogicalPresentation(renderer_.get(), &size.x, &size.y, &mode))
    {
        SDL_Log("%s", SDL_GetError());
        return getSize();
    }

    if (mode == SDL_LOGICAL_PRESENTATION_DISABLED || size.x <= 0 || size.y <= 0)
        return getSize();
    return size;
}

std::shared_ptr<SDL_Window> RenderWindow::getNativeSDLWindow()
{
    return window_;
}

void RenderWindow::convertEventToRenderCoordinates(SDL_Event *event) const
{
    if (!SDL_ConvertEventToRenderCoordinates(const_cast<SDL_Renderer *>(renderer_.get()), event))
        SDL_Log("%s", SDL_GetError());
}

void RenderWindow::convertEventToViewCoordinates(SDL_Event *event) const
{
    if (!event)
        return;

    const Vector2f screenCenter = getTargetCenter();
    // convertEventToRenderCoordinates(event);

    Matrix3x3<float> worldToScreen = getView().getTransformMatrix();
    worldToScreen.tx += screenCenter.x;
    worldToScreen.ty += screenCenter.y;

    Matrix3x3<float> screenToWorld;
    if (!worldToScreen.tryInverse(screenToWorld))
        return;

    auto transformPointInPlace = [&](float &x, float &y)
    {
        const Vector2f out = screenToWorld.transform({x, y});
        x = out.x;
        y = out.y;
    };

    auto transformDeltaInPlace = [&](float &x, float &y)
    {
        const Vector2f out = screenToWorld.transformVector({x, y});
        x = out.x;
        y = out.y;
    };

    switch (event->type)
    {
    case SDL_EVENT_MOUSE_MOTION:
        transformPointInPlace(event->motion.x, event->motion.y);
        transformDeltaInPlace(event->motion.xrel, event->motion.yrel);
        break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
        transformPointInPlace(event->button.x, event->button.y);
        break;

    case SDL_EVENT_FINGER_DOWN:
    case SDL_EVENT_FINGER_UP:
    case SDL_EVENT_FINGER_MOTION:
    case SDL_EVENT_FINGER_CANCELED:
        transformPointInPlace(event->tfinger.x, event->tfinger.y);
        transformDeltaInPlace(event->tfinger.dx, event->tfinger.dy);
        break;

    case SDL_EVENT_PEN_MOTION:
        transformPointInPlace(event->pmotion.x, event->pmotion.y);
        break;

    case SDL_EVENT_PEN_DOWN:
    case SDL_EVENT_PEN_UP:
        transformPointInPlace(event->ptouch.x, event->ptouch.y);
        break;

    case SDL_EVENT_PEN_BUTTON_DOWN:
    case SDL_EVENT_PEN_BUTTON_UP:
        transformPointInPlace(event->pbutton.x, event->pbutton.y);
        break;

    case SDL_EVENT_PEN_AXIS:
        transformPointInPlace(event->paxis.x, event->paxis.y);
        break;

    default:
        break;
    }
}

} // namespace sdl3
