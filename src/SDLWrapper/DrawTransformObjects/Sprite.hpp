#pragma once

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDLWrapper/ObjectBase/Drawable.hpp>
#include <SDLWrapper/ObjectBase/Transformable.hpp>
#include <SDLWrapper/Renders/RenderTarget.hpp>
#include <SDLWrapper/Texture.hpp>

namespace sdl3
{
class Sprite : public Drawable, public Transformable
{
public:
    Sprite() = default;

    void setFilterColor(const SDL_FColor& color)
    {
        color_ = color;
    }
    const SDL_FColor& getFilterColor() const
    {
        return color_;
    }

    void setTexture(const Texture &texture)
    {
        texture_ = &texture;
        textureRect_.x = 0;
        textureRect_.y = 0;
        SDL_Point size = texture.getSize();
        textureRect_.w = size.x;
        textureRect_.h = size.y;
        updateLocalGeometry();
    }
    void setTexture(const Texture &texture, const SDL_FRect &textureRect)
    {
        texture_ = &texture;
        textureRect_ = textureRect;
        updateLocalGeometry();
    }
    void setTextureRect(const SDL_FRect &textureRect)
    {
        textureRect_ = textureRect;
        updateLocalGeometry();
    }

    const Texture *getTexture() const
    {
        return texture_;
    }
    const SDL_FRect &getTextureRect() const
    {
        return textureRect_;
    }

    void setCenterPosition(const SDL_FPoint &position)
    {
        if (!texture_)
            return;
        SDL_FPoint pos = position;
        SDL_Point size = texture_->getSize();
        pos.x -= size.x / 2.f;
        pos.y -= size.y / 2.f;
        Transformable::setPosition(pos);
    }
    SDL_FPoint getCenterPosition() const
    {
        if (!texture_)
            return getPosition();
        SDL_FPoint pos = getPosition();
        SDL_Point size = texture_->getSize();
        pos.x += size.x / 2.f;
        pos.y += size.y / 2.f;
        return pos;
    }

private:
    const Texture *texture_;
    SDL_FRect textureRect_; // Какую часть текстуры отрисовать

    SDL_FColor color_ = {1.f, 1.f, 1.f, 1.f};

    SDL_FPoint localVertices_[4]{};
    SDL_FPoint textureUV_[4]{};

    mutable SDL_FPoint vertices_[4]{};
    mutable bool dirty_ = false;
    mutable unsigned viewId_ = 0;

    static constexpr const int indices_[6] = {0, 1, 2, 2, 3, 0};

private:
    void draw(RenderTarget &target) const override
    {
        if (!texture_)
            return;

        if (viewId_ != target.getViewId() || m_dirty || dirty_)
        {
            Matrix3x3 matrix = target.getView().getTransformMatrix() * getTransformMatrix();
            const SDL_FPoint screenCenter = target.getTargetCenter();
            matrix.tx += screenCenter.x;
            matrix.ty += screenCenter.y;
            updateVertices(matrix);
            viewId_ = target.getViewId();
        }
        target.drawShape(texture_, vertices_, 4, textureUV_, 4, color_, indices_, 6);
    }

    void updateLocalGeometry()
    {
        // 1. Позиции (простой прямоугольник от 0 до W/H)
        localVertices_[0] = {0.f, 0.f};
        localVertices_[1] = {textureRect_.w, 0.f};
        localVertices_[2] = {textureRect_.w, textureRect_.h};
        localVertices_[3] = {0.f, textureRect_.h};

        // 2. UV-координаты (нормализуем textureRect относительно размера текстуры)
        SDL_Point texSize = texture_->getSize();
        float tw = static_cast<float>(texSize.x);
        float th = static_cast<float>(texSize.y);

        float u1 = textureRect_.x / tw;
        float v1 = textureRect_.y / th;
        float u2 = (textureRect_.x + textureRect_.w) / tw;
        float v2 = (textureRect_.y + textureRect_.h) / th;

        textureUV_[0] = {u1, v1};
        textureUV_[1] = {u2, v1};
        textureUV_[2] = {u2, v2};
        textureUV_[3] = {u1, v2};
        dirty_ = true;
    }

    void updateVertices(const Matrix3x3 &matrix) const
    {
        for (int i = 0; i < 4; ++i)
            vertices_[i] = matrix.transform(localVertices_[i]);
        dirty_ = false;
    }
};

} // namespace sdl3