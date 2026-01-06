#include <SDLWrapper/DrawTransformObjects/Sprite.hpp>

#include <SDLWrapper/Renders/RenderTarget.hpp>
#include <SDLWrapper/Texture.hpp>

namespace sdl3
{

void Sprite::setFilterColor(const Color &color)
{
    color_ = color;
}

const Color &Sprite::getFilterColor() const
{
    return color_;
}

void Sprite::setTexture(const Texture &texture)
{
    texture_ = &texture;
    textureRect_.x = 0;
    textureRect_.y = 0;
    Vector2i size = texture.getSize();
    textureRect_.w = static_cast<float>(size.x);
    textureRect_.h = static_cast<float>(size.y);
    updateLocalGeometry();
}

void Sprite::setTexture(const Texture &texture, const FloatRect &textureRect)
{
    texture_ = &texture;
    textureRect_ = textureRect;
    updateLocalGeometry();
}

void Sprite::setTextureRect(const FloatRect &textureRect)
{
    textureRect_ = textureRect;
    updateLocalGeometry();
}

const Texture *Sprite::getTexture() const
{
    return texture_;
}

const FloatRect &Sprite::getTextureRect() const
{
    return textureRect_;
}

void Sprite::setCenterPosition(const Vector2f &position)
{
    if (!texture_)
        return;
    Vector2f pos = position;
    Vector2i size = texture_->getSize();
    pos.x -= size.x / 2.f;
    pos.y -= size.y / 2.f;
    Transformable::setPosition(pos);
}

Vector2f Sprite::getCenterPosition() const
{
    if (!texture_)
        return getPosition();
    Vector2f pos = getPosition();
    Vector2i size = texture_->getSize();
    pos.x += size.x / 2.f;
    pos.y += size.y / 2.f;
    return pos;
}

void Sprite::draw(RenderTarget &target) const
{
    if (!texture_)
        return;

    if (viewID_ != target.getViewId() || isGeometryDirty() || dirty_)
    {
        Matrix3x3<float> matrix = target.getView().getTransformMatrix() * getTransformMatrix();
        const Vector2f screenCenter = target.getTargetCenter();
        matrix.tx += screenCenter.x;
        matrix.ty += screenCenter.y;
        updateVertices(matrix);
        viewID_ = target.getViewId();
        updateGeometryVersion();
    }
    target.drawShape(texture_, vertices_, 4, textureUV_, 4, color_, indices_, 6);
}

void Sprite::updateLocalGeometry()
{
    localVertices_[0] = {0.f, 0.f};
    localVertices_[1] = {textureRect_.w, 0.f};
    localVertices_[2] = {textureRect_.w, textureRect_.h};
    localVertices_[3] = {0.f, textureRect_.h};

    if (!texture_)
    {
        for (auto &p : textureUV_)
            p = {0.f, 0.f};
        dirty_ = true;
        return;
    }

    Vector2i texSize = texture_->getSize();
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

void Sprite::updateVertices(const Matrix3x3<float> &matrix) const
{
    for (int i = 0; i < 4; ++i)
        vertices_[i] = matrix.transform(localVertices_[i]);
    dirty_ = false;
}

} // namespace sdl3

