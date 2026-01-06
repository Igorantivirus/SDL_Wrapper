#include <SDLWrapper/ObjectBase/Transformable.hpp>

#include <cmath>

#include <SDL3/SDL_stdinc.h>

namespace sdl3
{

const Matrix3x3<float> &Transformable::getTransformMatrix() const
{
    if (m_dirty)
    {
        float angle = rotation_ * (SDL_PI_F / 180.0f);
        float cosA = std::cos(angle);
        float sinA = std::sin(angle);

        float sxc = scale_.x * cosA;
        float sxs = scale_.x * sinA;
        float syc = scale_.y * cosA;
        float sys = scale_.y * sinA;

        matrix_.a = sxc;
        matrix_.b = sxs;
        matrix_.c = -sys;
        matrix_.d = syc;

        matrix_.tx = -origin_.x * sxc + origin_.y * sys + position_.x;
        matrix_.ty = -origin_.x * sxs - origin_.y * syc + position_.y;

        m_dirty = false;
    }
    return matrix_;
}

void Transformable::setPosition(const Vector2f &position)
{
    if (position_.x == position.x && position_.y == position.y)
        return;
    position_ = position;
    m_dirty = true;
}

void Transformable::setOriginKeepPosition(const Vector2f &newOrigin)
{
    if (origin_.x == newOrigin.x && origin_.y == newOrigin.y)
        return;
    const float dx = (newOrigin.x - origin_.x) * scale_.x;
    const float dy = (newOrigin.y - origin_.y) * scale_.y;

    const float rad = (-rotation_) * SDL_PI_F / 180.0f;

    const float c = std::cos(rad);
    const float s = std::sin(rad);

    const float rdx = dx * c - dy * s;
    const float rdy = dx * s + dy * c;

    position_.x += rdx;
    position_.y += rdy;

    origin_ = newOrigin;
    m_dirty = true;
}

void Transformable::setOrigin(const Vector2f &origin)
{
    if (origin_.x == origin.x && origin_.y == origin.y)
        return;
    origin_ = origin;
    m_dirty = true;
}

void Transformable::setScale(const Vector2f &scale)
{
    if (scale_.x == scale.x && scale_.y == scale.y)
        return;
    scale_.x = std::abs(scale.x);
    scale_.y = std::abs(scale.y);
    m_dirty = true;
}

void Transformable::setUniformScale(const float scale)
{
    if (scale_.x == scale && scale_.y == scale)
        return;
    float absScale = std::abs(scale);
    scale_.x = absScale;
    scale_.y = absScale;
    m_dirty = true;
}

void Transformable::setRotation(const float rotation)
{
    if (rotation == rotation_)
        return;
    rotation_ = std::fmod(rotation, 360.0f);
    m_dirty = true;
}

void Transformable::rotate(const float angle)
{
    if (angle == 0.f)
        return;
    setRotation(rotation_ + angle);
}

void Transformable::move(const Vector2f &offset)
{
    if (offset.x == 0.f && offset.y == 0.f)
        return;
    position_.x += offset.x;
    position_.y += offset.y;
    m_dirty = true;
}

void Transformable::scale(const Vector2f &factor)
{
    if (factor.x == 1.f && factor.y == 1.f)
        return;
    scale_.x *= std::abs(factor.x);
    scale_.y *= std::abs(factor.y);
    m_dirty = true;
}

void Transformable::uniformeScale(const float factor)
{
    if (factor == 1.f)
        return;
    scale_.x *= std::abs(factor);
    scale_.y *= std::abs(factor);
    m_dirty = true;
}

void Transformable::reset()
{
    position_ = {0.0f, 0.0f};
    origin_ = {0.0f, 0.0f};
    scale_ = {1.0f, 1.0f};
    rotation_ = 0.0f;
    m_dirty = true;
}

const Vector2f &Transformable::getPosition() const
{
    return position_;
}

const Vector2f &Transformable::getOrigin() const
{
    return origin_;
}

const Vector2f &Transformable::getScale() const
{
    return scale_;
}

float Transformable::getRotation() const
{
    return rotation_;
}

} // namespace sdl3

