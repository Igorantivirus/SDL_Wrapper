#pragma once

#include <SDL3/SDL_pixels.h>
#include <SDLWrapper/Rect.hpp>
#include <SDLWrapper/Names.hpp>
#include <SDLWrapper/Math/Colors.hpp>
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

    void setFilterColor(const Color &color);
    const Color &getFilterColor() const;

    void setTexture(const Texture &texture);
    void setTexture(const Texture &texture, const FloatRect &textureRect);
    void setTextureRect(const FloatRect &textureRect);

    const Texture *getTexture() const;
    const FloatRect &getTextureRect() const;

    void setCenterPosition(const Vector2f &position);
    Vector2f getCenterPosition() const;

private:
    const Texture *texture_ = nullptr;
    FloatRect textureRect_{};

    Color color_ = Colors::White;

    Vector2f localVertices_[4]{};
    Vector2f textureUV_[4]{};

    mutable Vector2f vertices_[4]{};
    mutable bool dirty_ = false;

    static constexpr int indices_[6] = {0, 1, 2, 2, 3, 0};

private:
    void draw(RenderTarget &target) const override;
    void updateLocalGeometry();
    void updateVertices(const Matrix3x3<float> &matrix) const;
};

} // namespace sdl3