#pragma once

#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_stdinc.h>
#include <memory>
#include <vector>

#include <SDL3/SDL_render.h>

#include <SDLWrapper/ObjectBase/Drawable.hpp>

// #include "ObjectBase/Transformable.hpp"
#include "SDLWrapper/ObjectBase/Transformable.hpp"
#include "View.hpp"
#include <SDLWrapper/Texture.hpp>

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

    void debug()
    {
        SDL_SetRenderScale(renderer_.get(), 2, 2);
    }

    void drawTexture(const Texture &texture, const Transformable &tr, const SDL_FRect &srcRect)
    {
        auto sp = texture.getSDLTexture().lock();
        if (!sp)
            return;
        SDL_Texture *sdlTex = const_cast<SDL_Texture *>(sp.get());

        const SDL_FPoint pivotPos = tr.getPosition(); // pivot/origin position in world
        const SDL_FPoint scale = tr.getScale();
        const SDL_FPoint origin = tr.getOrigin();
        const float rotDeg = tr.getRotation();

        const SDL_FPoint zoom = view_.getZoom();
        const float sx = scale.x * zoom.x;
        const float sy = scale.y * zoom.y;

        // Экранная позиция pivot
        SDL_FPoint screenPivot = view_.isDefault() ? pivotPos : worldToScreen(pivotPos);

        // Pivot внутри dstRect (в пикселях dstRect)
        SDL_FPoint center;
        center.x = origin.x * sx;
        center.y = origin.y * sy;

        SDL_FRect dst;
        dst.w = srcRect.w * sx;
        dst.h = srcRect.h * sy;

        // ВАЖНОЕ отличие: dstRect строим так, чтобы pivot оказался в screenPivot
        dst.x = screenPivot.x - center.x;
        dst.y = screenPivot.y - center.y;

        // Быстрый путь: если нет поворота и камера default, можно без Rotated,
        // но смещение по origin всё равно нужно, иначе pivot "уедет".
        if (view_.isDefault() && rotDeg == 0.0f)
        {
            SDL_RenderTexture(renderer_.get(), sdlTex, &srcRect, &dst);
            return;
        }

        const double angle = double(rotDeg - view_.getAngle());

        SDL_RenderTextureRotated(renderer_.get(), sdlTex, &srcRect, &dst, angle, &center, SDL_FLIP_NONE);
    }

    void drawShape(const std::vector<SDL_Vertex> &fillVertices, const std::vector<SDL_Vertex> &outlineVertices, const Texture *texture)
    {
        if (!renderer_)
            return;

        const SDL_Texture *sdlTex = nullptr;
        if (texture)
        {
            auto sp = texture->getSDLTexture().lock();
            if (sp)
                sdlTex = sp.get();
        }

        if (!fillVertices.empty())
        {
            SDL_RenderGeometry(renderer_.get(), const_cast<SDL_Texture *>(sdlTex), fillVertices.data(), static_cast<int>(fillVertices.size()), nullptr, 0);
        }

        if (!outlineVertices.empty())
        {
            SDL_RenderGeometry(renderer_.get(), nullptr, outlineVertices.data(), static_cast<int>(outlineVertices.size()), nullptr, 0);
        }
    }

    const View &getView() const
    {
        return view_;
    }
    void setView(const View &view)
    {
        view_ = view;
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

protected:
    void setBaseViewPosition(const SDL_FPoint &pos)
    {
        view_.setCenterPosition(pos);
        view_.baseCenter_ = pos;
    }
    SDL_FPoint worldToScreen(const SDL_FPoint &worldPos) const
    {
        SDL_FPoint targetSize{(float)targetSize_.x, (float)targetSize_.y};

        float relX = worldPos.x - view_.getCenterPosition().x;
        float relY = worldPos.y - view_.getCenterPosition().y;

        float rotatedX = relX * view_.cosAngle_ + relY * view_.sinAngle_;
        float rotatedY = -relX * view_.sinAngle_ + relY * view_.cosAngle_;

        float screenX = (rotatedX * view_.getZoom().x) + (targetSize.x / 2.0f);
        float screenY = (rotatedY * view_.getZoom().y) + (targetSize.y / 2.0f);

        return SDL_FPoint{screenX, screenY};
    }
};

} // namespace sdl3
