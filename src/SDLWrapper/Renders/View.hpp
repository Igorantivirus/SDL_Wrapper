#pragma once

#include <cmath>

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_stdinc.h>

namespace sdl3
{

class RenderTarget;

class View
{
    friend class RenderTarget;

public:
    // ============ Центр ============
    void setCenterPosition(const SDL_FPoint &pos)
    {
        center_ = pos;
    }

    const SDL_FPoint &getCenterPosition() const
    {
        return center_;
    }

    // ============ Масштаб ============
    void setZoom(const SDL_FPoint &zoom)
    {
        // Защита от отрицательного масштаба
        zoom_.x = std::abs(zoom.x);
        zoom_.y = std::abs(zoom.y);
    }

    void setUniformScale(float zoom)
    {
        float positiveScale = std::abs(zoom);
        zoom_.x = positiveScale;
        zoom_.y = positiveScale;
    }

    const SDL_FPoint &getZoom() const
    {
        return zoom_;
    }

    // ============ Угол ============
    void setAngle(float degrees)
    {
        angle_ = std::fmod(degrees, 360.0f);

        static const constexpr float TO_RAD = SDL_PI_F / 180.f;

        sinAngle_ = std::sin(angle_ * TO_RAD);
        cosAngle_ = std::cos(angle_ * TO_RAD);
    }

    float getAngle() const
    {
        return angle_;
    }

    // ============ Проверка на значения по умолчанию ============
    bool isDefault() const
    {
        return center_.x == baseCenter_.x && center_.y == baseCenter_.y &&
               zoom_.x == 1.f && zoom_.y == 1.f &&
               angle_ == 0.f;
    }

    // ============ Сброс к значениям по умолчанию ============
    void reset()
    {
        center_.x = 0.0f;
        center_.y = 0.0f;
        zoom_.x = 1.0f;
        zoom_.y = 1.0f;
        angle_ = 0.0f;
    }

    // ============ Дополнительные полезные методы ============
    bool isZeroZoom() const
    {
        return zoom_.x == 0.0f && zoom_.y == 0.0f;
    }

    bool isUniformZoom() const
    {
        return zoom_.x == zoom_.y;
    }

    void resetView()
    {
        center_ = {0.0f, 0.0f};
        zoom_ = {1.f, 1.f};
        angle_ = 0.f;    // Градусы
        sinAngle_ = 0.f; // Синус угла
        cosAngle_ = 1.f; // Косинус угла
    }

    bool isDefaultView() const
    {
        return 
        center_.x == 0.f && 
        center_.y == 0.f && 
        zoom_.x == 0.f && 
        zoom_.y == 0.f &&
        angle_ == 0.f; 
    }

    void rotate(const float angle)
    {
        setAngle(angle_ + angle);
    }


private:
    SDL_FPoint center_{0.0f, 0.0f};
    SDL_FPoint zoom_ = {1.f, 1.f};

    float angle_ = 0.f;    // Градусы

    float sinAngle_ = 0.f; // Синус угла
    float cosAngle_ = 1.f; // Косинус угла

    SDL_FPoint baseCenter_ = {};
};

} // namespace sdl3