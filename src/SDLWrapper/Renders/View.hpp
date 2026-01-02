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
        float radians = angle_ * (SDL_PI_F / 180.0f);
        sinAngle_ = std::sin(radians);
        cosAngle_ = std::cos(radians);
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

            float a = cosAngle_ * zoom_.x;
            float b = sinAngle_ * zoom_.x;
            float c = -sinAngle_ * zoom_.y;
            float d = cosAngle_ * zoom_.y;

            matrix_.a = a;
            matrix_.b = b;
            matrix_.c = c;
            matrix_.d = d;

            // Сдвиг: точка center_ должна стать точкой (0,0)
            matrix_.tx = -center_.x * a - center_.y * c;
            matrix_.ty = -center_.x * b - center_.y * d;

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
        sinAngle_ = 0.0f;
        cosAngle_ = 1.0f;
        m_dirty = true;
    }

    bool isDefaultView() const
    {
        return center_.x == 0.f && center_.y == 0.f &&
               zoom_.x == 1.f && zoom_.y == 1.f &&
               angle_ == 0.f;
    }

private:
    SDL_FPoint center_{0.0f, 0.0f};
    SDL_FPoint zoom_ = {1.f, 1.f};
    float angle_ = 0.f;

    float sinAngle_ = 0.f;
    float cosAngle_ = 1.f;

    mutable Matrix3x3 matrix_;
    mutable bool m_dirty = true;
};

} // namespace sdl3