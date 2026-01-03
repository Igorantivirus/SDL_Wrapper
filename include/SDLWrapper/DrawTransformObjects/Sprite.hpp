#pragma once

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>

#include <SDLWrapper/ObjectBase/Drawable.hpp>
#include <SDLWrapper/ObjectBase/Transformable.hpp>

namespace sdl3
{

class RenderTarget;
class Texture;

class Sprite : public Drawable, public Transformable
{
public:
    Sprite() = default;

    void setFilterColor(const SDL_FColor &color);
    const SDL_FColor &getFilterColor() const;

    void setTexture(const Texture &texture);
    void setTexture(const Texture &texture, const SDL_FRect &textureRect);
    void setTextureRect(const SDL_FRect &textureRect);

    const Texture *getTexture() const;
    const SDL_FRect &getTextureRect() const;

    void setCenterPosition(const SDL_FPoint &position);
    SDL_FPoint getCenterPosition() const;

private:
    const Texture *texture_ = nullptr;
    SDL_FRect textureRect_{};

    SDL_FColor color_ = {1.f, 1.f, 1.f, 1.f};

    SDL_FPoint localVertices_[4]{};
    SDL_FPoint textureUV_[4]{};

    mutable SDL_FPoint vertices_[4]{};
    mutable bool dirty_ = false;

    static constexpr int indices_[6] = {0, 1, 2, 2, 3, 0};

private:
    void draw(RenderTarget &target) const override;
    void updateLocalGeometry();
    void updateVertices(const Matrix3x3 &matrix) const;
};

} // namespace sdl3