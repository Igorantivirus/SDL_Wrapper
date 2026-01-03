#pragma once

#include <vector>

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>

#include <SDLWrapper/ObjectBase/Drawable.hpp>
#include <SDLWrapper/ObjectBase/Transformable.hpp>

namespace sdl3
{

class RenderTarget;
class Texture;

class Shape : public Drawable, public Transformable
{
public:
    virtual ~Shape() = default;

    const SDL_FColor &getFillColor() const;
    const SDL_FColor &getOutlineColor() const;
    float getOutlineThickness() const;
    const Texture *getTexture() const;
    const SDL_FRect &getTextureRect() const;

    virtual SDL_FPoint getPoint(std::size_t index) const = 0;
    virtual std::size_t getPointCount() const = 0;

    void setFillColor(const SDL_FColor &color);
    void setOutlineColor(const SDL_FColor &color);
    void setOutlineThickness(float outlineThickness);

    void setTexture(const Texture &texture, const SDL_FRect &rect);
    void setTexture(const Texture &texture);
    void setTextureRect(const SDL_FRect &rect);

protected:
    void updateLocalGeometry();

private:
    const Texture *texture_ = nullptr;
    SDL_FRect textureRect_ = {0, 0, 0, 0};

    SDL_FColor fillColor_ = {1.0f, 1.0f, 1.0f, 1.0f};
    SDL_FColor outlineColor_ = {1.0f, 1.0f, 1.0f, 1.0f};
    float outlineThickness_ = 0.0f;

    SDL_FRect localBounds_{};
    std::vector<SDL_FPoint> textureUV_;
    std::vector<SDL_FPoint> localVertices_;
    std::vector<SDL_FPoint> localOutlineVertices_;

    mutable std::vector<SDL_FPoint> vertices_;
    mutable std::vector<SDL_FPoint> outlineVertices_;
    mutable bool shapeDirty_ = true;
    mutable bool outlineDirty_ = true;

private:
    void draw(RenderTarget &target) const override;

    void updateLocalShape();
    void updateLocalOutline();
    void updateLocalBounds();
    void updateTexturePoints();

    void updateVertices(const Matrix3x3 &matrix) const;
    void updateOutlineVertices(const Matrix3x3 &matrix) const;
};

} // namespace sdl3