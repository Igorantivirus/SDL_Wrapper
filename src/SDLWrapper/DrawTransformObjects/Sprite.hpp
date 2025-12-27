#pragma once

#include <SDL3/SDL_rect.h>
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
    }
    void setTexture(const Texture &texture, const SDL_FRect &textureRect)
    {
        texture_ = &texture;
        textureRect_ = textureRect;
    }
    void setTextureRect(const SDL_FRect &textureRect)
    {
        textureRect_ = textureRect;
    }

    const Texture *getTexture() const
    {
        return texture_;
    }
    const SDL_FRect &getTextureRect() const
    {
        return textureRect_;
    }

    void setCenterPosition(const SDL_FPoint &position) override
    {
        if (!texture_)
            return;
        SDL_FPoint pos = position;
        SDL_Point size = texture_->getSize();
        pos.x -= size.x / 2.f;
        pos.y -= size.y / 2.f;
        Transformable::setPosition(pos);
    }
    SDL_FPoint getCenterPosition() const override
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

private:
    void draw(RenderTarget &target) const override
    {
        if (!texture_)
            return;
        target.drawTexture(*texture_, *this, textureRect_);
    }
};

} // namespace sdl3