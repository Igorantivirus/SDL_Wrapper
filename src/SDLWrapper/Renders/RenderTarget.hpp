#pragma once

#include <memory>

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>

#include <SDLWrapper/ObjectBase/Drawable.hpp>
#include <SDLWrapper/ObjectBase/Transformable.hpp>
#include <SDLWrapper/Texture.hpp>

#include "View.hpp"

namespace sdl3
{

class RenderTarget
{
public:
    virtual ~RenderTarget() = default;

    void draw(const Drawable &object)
    {
        object.draw(*this);
    }

    void drawShape(const Texture *texture,
                   const SDL_FPoint *positions, const int posCnt,
                   const SDL_FPoint *uv, const int uvCnt,
                   const SDL_FColor &color,
                   const int *indices, const int indCnt
                )
    {
        if (!renderer_ || !posCnt)
            return;
        SDL_Texture *sdlTex = getRawTextureFromTexture(texture);
        SDL_RenderGeometryRaw(renderer_.get(), sdlTex,
                              &positions->x, sizeof(SDL_FPoint),
                              &color, 0,
                              &uv->x, sizeof(SDL_FPoint),
                              posCnt,
                              indices, indCnt, sizeof(int));
    }

    const View &getView() const
    {
        return view_;
    }
    void setView(const View &view)
    {
        if (view_ != view)
        {
            view_ = view;
            ++viewId_;
        }
    }
    unsigned getViewId() const
    {
        return viewId_;
    }
    SDL_FPoint getTargetCenter() const
    {
        return SDL_FPoint{targetSize_.x / 2.0f, targetSize_.y / 2.0f};
    }

    void clear(const SDL_Color &color = SDL_Color{0, 0, 0, 255})
    {
        SDL_SetRenderDrawColor(renderer_.get(), color.r, color.g, color.b, color.a);
        SDL_RenderClear(renderer_.get());
    }
    void display()
    {
        SDL_RenderPresent(renderer_.get());
    }

protected:
    std::shared_ptr<SDL_Renderer> renderer_;
    View view_;
    SDL_Point targetSize_{};

    unsigned viewId_ = 1;

protected:
    void setBaseViewPosition(const SDL_FPoint &pos)
    {
        view_.setCenterPosition(pos);
        // view_.setCenterPosition(const );
    }
    SDL_FPoint worldToScreen(const SDL_FPoint &worldPos) const
    {
        SDL_FPoint targetSize{(float)targetSize_.x, (float)targetSize_.y};

        float relX = worldPos.x - view_.getCenterPosition().x;
        float relY = worldPos.y - view_.getCenterPosition().y;

        float rotatedX = 0; // relX * view_.cosAngle_ + relY * view_.sinAngle_;
        float rotatedY = 0; //-relX * view_.sinAngle_ + relY * view_.cosAngle_;

        float screenX = (rotatedX * view_.getZoom().x) + (targetSize.x / 2.0f);
        float screenY = (rotatedY * view_.getZoom().y) + (targetSize.y / 2.0f);

        return SDL_FPoint{screenX, screenY};
    }

    SDL_Texture *getRawTextureFromTexture(const Texture *texture)
    {
        if (texture)
            return const_cast<SDL_Texture *>(texture->getSDLTexture().lock().get());
        return nullptr;
    }
};

} // namespace sdl3
