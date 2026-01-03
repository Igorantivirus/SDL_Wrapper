#pragma once

#include <SDL3/SDL_stdinc.h>
#include <cmath>
#include <cstdlib>

#include <SDL3/SDL_rect.h>

#include <SDLWrapper/Math/Matrix3x3.hpp>

namespace sdl3
{

class Transformable
{
public:
    virtual ~Transformable() = default;

    const Matrix3x3 &getTransformMatrix() const
    {
        if (m_dirty)
        {
            float angle = rotation_ * (SDL_PI_F / 180.0f); // переводим в радианы
            float cosA = std::cos(angle);
            float sinA = std::sin(angle);

            // Коэффициенты с учетом Scale и Rotation
            float sxc = scale_.x * cosA;
            float sxs = scale_.x * sinA;
            float syc = scale_.y * cosA;
            float sys = scale_.y * sinA;

            // Матрица трансформации:
            matrix_.a = sxc;
            matrix_.b = sxs;
            matrix_.c = -sys;
            matrix_.d = syc;

            // Учитываем Origin и Position (tx, ty)
            matrix_.tx = -origin_.x * sxc + origin_.y * sys + position_.x;
            matrix_.ty = -origin_.x * sxs - origin_.y * syc + position_.y;

            m_dirty = false;
        }
        return matrix_;
    }

    void setPosition(const SDL_FPoint &position)
    {
        if(position_.x == position.x && position_.y == position.y)
            return;
        position_ = position;
        m_dirty = true;
    }
    void setOriginKeepPosition(const SDL_FPoint &newOrigin)
    {
        if(origin_.x == newOrigin.x && origin_.y == newOrigin.y)
            return;
        const float dx = (newOrigin.x - origin_.x) * scale_.x;
        const float dy = (newOrigin.y - origin_.y) * scale_.y;

        const float rad = (-rotation_) * 3.14159265358979323846f / 180.0f;

        const float c = std::cos(rad);
        const float s = std::sin(rad);

        const float rdx = dx * c - dy * s;
        const float rdy = dx * s + dy * c;

        position_.x += rdx;
        position_.y += rdy;

        origin_ = newOrigin;
        m_dirty = true;
    }
    void setOrigin(const SDL_FPoint &origin)
    {
        if(origin_.x == origin.x && origin_.y == origin.y)
            return;
        origin_ = origin;
        m_dirty = true;
    }
    void setScale(const SDL_FPoint &scale)
    {
        if(scale_.x == scale.x && scale_.y == scale.y)
            return;
        scale_.x = std::abs(scale.x);
        scale_.y = std::abs(scale.y);
        m_dirty = true;
    }
    void setUniformScale(float scale)
    {
        if(scale_.x == scale && scale_.y == scale)
            return;
        float absScale = std::abs(scale);
        scale_.x = absScale;
        scale_.y = absScale;
        m_dirty = true;
    }
    void setRotation(float rotation)
    {
        if(rotation == rotation_)
            return;
        rotation_ = std::fmod(rotation, 360.0f);
        m_dirty = true;
    }

    void rotate(float angle)
    {
        if(angle == 0.f)
            return;
        setRotation(rotation_ + angle);
    }
    void move(const SDL_FPoint &offset)
    {
        if(offset.x == 0.f && offset.y == 0.f)
            return;
        position_.x += offset.x;
        position_.y += offset.y;
        m_dirty = true;
    }
    void scale(const SDL_FPoint &factor)
    {
        if(factor.x == 1.f && factor.y == 1.f)
            return;
        scale_.x *= std::abs(factor.x);
        scale_.y *= std::abs(factor.y);
        m_dirty = true;
    }

    void uniformeScale(const float factor)
    {
        if(factor == 1.f)
            return;
        scale_.x *= std::abs(factor);
        scale_.y *= std::abs(factor);
        m_dirty = true;
    }
    void reset()
    {
        position_ = {0.0f, 0.0f};
        origin_ = {0.0f, 0.0f};
        scale_ = {1.0f, 1.0f};
        rotation_ = 0.0f;
        m_dirty = true;
    }

    const SDL_FPoint &getPosition() const
    {
        return position_;
    }
    const SDL_FPoint &getOrigin() const
    {
        return origin_;
    }
    const SDL_FPoint &getScale() const
    {
        return scale_;
    }
    float getRotation() const
    {
        return rotation_;
    }

protected:
    SDL_FPoint position_ = {0.0f, 0.0f};
    SDL_FPoint origin_ = {0.0f, 0.0f};
    SDL_FPoint scale_ = {1.0f, 1.0f};
    float rotation_ = 0.0f;

    mutable Matrix3x3 matrix_;
    mutable bool m_dirty = true;

};

} // namespace sdl3
