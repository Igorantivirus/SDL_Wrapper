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

    SDL_Vertex localVertices_[4]{};
    mutable SDL_Vertex vertices_[4]{};
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
        target.drawShape(vertices_, 4, texture_, indices_, 6);
    }

    void updateLocalGeometry()
    {
        // 1. Позиции (простой прямоугольник от 0 до W/H)
        localVertices_[0].position = {0.f, 0.f};
        localVertices_[1].position = {textureRect_.w, 0.f};
        localVertices_[2].position = {textureRect_.w, textureRect_.h};
        localVertices_[3].position = {0.f, textureRect_.h};

        // 2. UV-координаты (нормализуем textureRect относительно размера текстуры)
        SDL_Point texSize = texture_->getSize();
        float tw = static_cast<float>(texSize.x);
        float th = static_cast<float>(texSize.y);

        float u1 = textureRect_.x / tw;
        float v1 = textureRect_.y / th;
        float u2 = (textureRect_.x + textureRect_.w) / tw;
        float v2 = (textureRect_.y + textureRect_.h) / th;

        localVertices_[0].tex_coord = {u1, v1};
        localVertices_[1].tex_coord = {u2, v1};
        localVertices_[2].tex_coord = {u2, v2};
        localVertices_[3].tex_coord = {u1, v2};

        // 3. Цвет
        for (int i = 0; i < 4; ++i)
            localVertices_[i].color = SDL_FColor{1, 1, 1, 1};
    }

    void updateVertices(const Matrix3x3 &matrix) const
    {
        for (int i = 0; i < 4; ++i)
        {
            // Копируем UV и Цвет из локального эталона
            vertices_[i].tex_coord = localVertices_[i].tex_coord;
            vertices_[i].color = localVertices_[i].color;

            // Трансформируем только позицию
            vertices_[i].position = matrix.transform(localVertices_[i].position);
        }
    }
};

} // namespace sdl3