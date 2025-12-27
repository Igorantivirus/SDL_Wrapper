#pragma once

#include <cmath>
#include <cstdlib>

#include <SDL3/SDL_rect.h>

namespace sdl3
{

class Transformable
{
public:
    virtual ~Transformable() = default;

    void setPosition(const SDL_FPoint &position)
    {
        position_ = position;
    }
    void setOriginKeepPosition(const SDL_FPoint &newOrigin)
    {
        // deltaOrigin в локальных координатах (пиксели текстуры)
        const float dx = (newOrigin.x - origin_.x) * scale_.x;
        const float dy = (newOrigin.y - origin_.y) * scale_.y;

        // ВАЖНО про знак:
        // если rotation_ у тебя "как SDL" (плюс = по часовой),
        // то для обычных sin/cos (CCW) нужен минус.
        const float rad = (-rotation_) * 3.14159265358979323846f / 180.0f;

        const float c = std::cos(rad);
        const float s = std::sin(rad);

        // rotate( dx, dy )
        const float rdx = dx * c - dy * s;
        const float rdy = dx * s + dy * c;

        // компенсируем position_, чтобы объект в мире не сдвинулся
        position_.x += rdx;
        position_.y += rdy;

        // теперь можно менять origin
        origin_ = newOrigin;

        // если есть dirty-флаг/кеш — пометь
        // dirty_ = true;
    }
    virtual void setCenterPosition(const SDL_FPoint &position)
    {
    }
    void setOrigin(const SDL_FPoint &origin)
    {
        origin_ = origin;
    }
    void setScale(const SDL_FPoint &scale)
    {
        // Абсолютное значение для предотвращения отрицательного масштаба
        scale_.x = std::abs(scale.x);
        scale_.y = std::abs(scale.y);
        isTransformed_ = isTransformedCheck();
    }
    void setUniformScale(float scale)
    {
        float absScale = std::abs(scale);
        scale_.x = absScale;
        scale_.y = absScale;
        isTransformed_ = isTransformedCheck();
    }
    void setRotation(float rotation)
    {
        rotation_ = std::fmod(rotation, 360.0f);
        isTransformed_ = isTransformedCheck();
    }

    void rotate(float angle)
    {
        setRotation(rotation_ + angle);
    }
    void move(const SDL_FPoint &offset)
    {
        position_.x += offset.x;
        position_.y += offset.y;
    }
    void scale(const SDL_FPoint &factor)
    {
        scale_.x *= std::abs(factor.x);
        scale_.y *= std::abs(factor.y);
        isTransformed_ = isTransformedCheck();
    }

    void uniformeScale(const float factor)
    {
        scale_.x *= std::abs(factor);
        scale_.y *= std::abs(factor);
        isTransformed_ = isTransformedCheck();
    }
    void resetTransform()
    {
        position_ = {0.0f, 0.0f};
        origin_ = {0.0f, 0.0f};
        scale_ = {1.0f, 1.0f};
        rotation_ = 0.0f;
        isTransformed_ = false;
    }

    const SDL_FPoint &getPosition() const
    {
        return position_;
    }
    virtual SDL_FPoint getCenterPosition() const
    {
        return SDL_FPoint{position_.x, position_.y};
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

    bool isTransformed() const
    {
        return isTransformed_;
    }

protected:
    SDL_FPoint position_ = {0.0f, 0.0f}; // Координаты объекта в мире

    SDL_FPoint origin_ = {0.0f, 0.0f}; // Точка, относительно которой будет совершён поворот
    SDL_FPoint scale_ = {1.0f, 1.0f};  // Коэффициенты масштабирования по X и Y
    float rotation_ = 0.0f;            // Угол поворота в градусах (нормализованный в [-180, 180))

    bool isTransformed_ = false;

private:
    bool isTransformedCheck() const
    {
        return !(scale_.x == 1.0f && scale_.y == 1.0f && rotation_ == 0.0f);
    }
};

} // namespace sdl3