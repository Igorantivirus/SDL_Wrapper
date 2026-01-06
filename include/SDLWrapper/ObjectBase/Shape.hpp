#pragma once

#include <vector>

#include <SDLWrapper/Rect.hpp>
#include <SDLWrapper/Names.hpp>
#include <SDLWrapper/Math/Colors.hpp>
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

    const Color &getFillColor() const;
    const Color &getOutlineColor() const;
    float getOutlineThickness() const;
    const Texture *getTexture() const;
    const FloatRect &getTextureRect() const;

    virtual Vector2f getPoint(std::size_t index) const = 0;
    virtual std::size_t getPointCount() const = 0;

    void setFillColor(const Color &color);
    void setOutlineColor(const Color &color);
    void setOutlineThickness(float outlineThickness);

    void setTexture(const Texture &texture, const FloatRect &rect);
    void setTexture(const Texture &texture);
    void setTextureRect(const FloatRect &rect);

protected:
    void updateLocalGeometry();

private:
    const Texture *texture_ = nullptr;
    FloatRect textureRect_ = {0, 0, 0, 0};

    Color fillColor_ = {1.0f, 1.0f, 1.0f, 1.0f};
    Color outlineColor_ = {1.0f, 1.0f, 1.0f, 1.0f};
    float outlineThickness_ = 0.0f;

    FloatRect localBounds_{};
    std::vector<Vector2f> textureUV_;
    std::vector<Vector2f> localVertices_;
    std::vector<Vector2f> localOutlineVertices_;

    mutable std::vector<Vector2f> vertices_;
    mutable std::vector<Vector2f> outlineVertices_;
    mutable bool shapeDirty_ = true;
    mutable bool outlineDirty_ = true;

private:
    void draw(RenderTarget &target) const override;

    void updateLocalShape();
    void updateLocalOutline();
    void updateLocalBounds();
    void updateTexturePoints();

    void updateVertices(const Matrix3x3<float> &matrix) const;
    void updateOutlineVertices(const Matrix3x3<float> &matrix) const;
};

} // namespace sdl3