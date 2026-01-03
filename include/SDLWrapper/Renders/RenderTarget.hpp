#pragma once

#include <SDL3/SDL_render.h>

#include "SDLWrapper/Texture.hpp"
#include <memory>

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>

#include <SDLWrapper/Renders/View.hpp>

namespace sdl3
{


class Drawable;
class Texture;

class RenderTarget
{
public:
    virtual ~RenderTarget() = default;

    void draw(const Drawable &object);

    void drawShape(const Texture *texture,
                   const SDL_FPoint *positions, int posCnt,
                   const SDL_FPoint *uv, int uvCnt,
                   const SDL_FColor &color,
                   const int *indices, int indCnt);

    const View &getView() const;
    void setView(const View &view);
    unsigned getViewId() const;
    SDL_FPoint getTargetCenter() const;

    void clear(const SDL_Color &color = SDL_Color{0, 0, 0, 255});
    void display();

protected:
    std::shared_ptr<SDL_Renderer> renderer_;
    View view_;
    SDL_Point targetSize_{};

    unsigned viewId_ = 1;

protected:
    void setBaseViewPosition(const SDL_FPoint &pos);
};

} // namespace sdl3

