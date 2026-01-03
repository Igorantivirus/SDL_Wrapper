#pragma once

#include <cmath>

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_stdinc.h>

#include <SDLWrapper/Math/Matrix3x3.hpp>

namespace sdl3
{

class View
{
public:
    View() = default;

    // ============ Центр (Точка в мире, на которую смотрим) ============
    void setCenterPosition(const SDL_FPoint &pos)
    {
        if (center_.x == pos.x && center_.y == pos.y)
            return;
        center_ = pos;
        m_dirty = true;
    }

    const SDL_FPoint &getCenterPosition() const
    {
        return center_;
    }

    // ============ Масштаб (Zoom) ============
    void setZoom(const SDL_FPoint &zoom)
    {
        float zx = std::abs(zoom.x);
        float zy = std::abs(zoom.y);
        if (zoom_.x == zx && zoom_.y == zy)
            return;
        zoom_.x = zx;
        zoom_.y = zy;
        m_dirty = true;
    }

    void setUniformScale(float zoom)
    {
        float z = std::abs(zoom);
        if (zoom_.x == z && zoom_.y == z)
            return;
        zoom_.x = z;
        zoom_.y = z;
        m_dirty = true;
    }

    const SDL_FPoint &getZoom() const
    {
        return zoom_;
    }

    // ============ Угол поворота камеры ============
    void setAngle(float degrees)
    {
        float newAngle = std::fmod(degrees, 360.0f);
        if (angle_ == newAngle)
            return;
        angle_ = newAngle;
        m_dirty = true;
    }

    float getAngle() const
    {
        return angle_;
    }

    void rotate(const float angle)
    {
        setAngle(angle_ + angle);
    }

    // ============ Матрица трансформации View ============
    const Matrix3x3 &getTransformMatrix() const
    {
        if (m_dirty)
        {
            // Матрица Вида (World -> View space):
            // 1. Сдвигаем мир на -center_
            // 2. Вращаем (для камеры вращение мира идет в обратную сторону)
            // 3. Масштабируем

            // Расчет компонентов с учетом инверсии камеры:
            // Чтобы мир вращался корректно, используем стандартную матрицу,
            // которая применится к координатам (Point - Center)
            float radians = angle_ * (SDL_PI_F / 180.0f);
            float sinAngle = std::sin(radians);
            float cosAngle = std::cos(radians);

            matrix_.a = cosAngle * zoom_.x;
            matrix_.b = sinAngle * zoom_.x;
            matrix_.c = -sinAngle * zoom_.y;
            matrix_.d = cosAngle * zoom_.y;

            // Сдвиг: точка center_ должна стать точкой (0,0)
            matrix_.tx = -center_.x * matrix_.a - center_.y * matrix_.c;
            matrix_.ty = -center_.x * matrix_.b - center_.y * matrix_.d;

            m_dirty = false;
        }
        return matrix_;
    }

    // ============ Сброс ============
    void reset()
    {
        center_ = {0.0f, 0.0f};
        zoom_ = {1.0f, 1.0f};
        angle_ = 0.0f;
        m_dirty = true;
    }

    bool operator==(const View &other)
    {
        return center_.x == other.center_.x &&
               center_.y == other.center_.y &&
               zoom_.x == other.zoom_.x &&
               zoom_.y == other.zoom_.y &&
               angle_ == other.angle_;
    }
    bool operator!=(const View &other)
    {
        return !this->operator==(other);
    }

private:
    SDL_FPoint center_{0.0f, 0.0f};
    SDL_FPoint zoom_ = {1.f, 1.f};
    float angle_ = 0.f;

    mutable Matrix3x3 matrix_;
    mutable bool m_dirty = true;
};

} // namespace sdl3