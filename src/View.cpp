#include <SDLWrapper/Renders/View.hpp>

#include <cmath>

#include <SDL3/SDL_stdinc.h>

namespace sdl3
{

void View::setCenterPosition(const Vector2f &pos)
{
    if (center_.x == pos.x && center_.y == pos.y)
        return;
    center_ = pos;
    m_dirty = true;
}

const Vector2f &View::getCenterPosition() const
{
    return center_;
}

void View::setZoom(const Vector2f &zoom)
{
    float zx = std::abs(zoom.x);
    float zy = std::abs(zoom.y);
    if (zoom_.x == zx && zoom_.y == zy)
        return;
    zoom_.x = zx;
    zoom_.y = zy;
    m_dirty = true;
}

void View::setUniformScale(const float zoom)
{
    float z = std::abs(zoom);
    if (zoom_.x == z && zoom_.y == z)
        return;
    zoom_.x = z;
    zoom_.y = z;
    m_dirty = true;
}

const Vector2f &View::getZoom() const
{
    return zoom_;
}

void View::setAngle(const float degrees)
{
    float newAngle = std::fmod(degrees, 360.0f);
    if (angle_ == newAngle)
        return;
    angle_ = newAngle;
    m_dirty = true;
}

float View::getAngle() const
{
    return angle_;
}

void View::rotate(const float angle)
{
    setAngle(angle_ + angle);
}

const Matrix3x3<float> &View::getTransformMatrix() const
{
    if (m_dirty)
    {
        float radians = angle_ * (SDL_PI_F / 180.0f);
        float sinAngle = std::sin(radians);
        float cosAngle = std::cos(radians);

        matrix_.a = cosAngle * zoom_.x;
        matrix_.b = sinAngle * zoom_.x;
        matrix_.c = -sinAngle * zoom_.y;
        matrix_.d = cosAngle * zoom_.y;

        matrix_.tx = -center_.x * matrix_.a - center_.y * matrix_.c;
        matrix_.ty = -center_.x * matrix_.b - center_.y * matrix_.d;

        m_dirty = false;
    }
    return matrix_;
}

void View::reset()
{
    center_ = {0.0f, 0.0f};
    zoom_ = {1.0f, 1.0f};
    angle_ = 0.0f;
    m_dirty = true;
}

bool View::operator==(const View &other)
{
    return center_.x == other.center_.x &&
           center_.y == other.center_.y &&
           zoom_.x == other.zoom_.x &&
           zoom_.y == other.zoom_.y &&
           angle_ == other.angle_;
}

bool View::operator!=(const View &other)
{
    return !this->operator==(other);
}

} // namespace sdl3

