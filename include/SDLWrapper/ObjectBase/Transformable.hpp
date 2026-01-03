#pragma once

#include <SDL3/SDL_rect.h>

#include <SDLWrapper/Math/Matrix3x3.hpp>

namespace sdl3
{

class Transformable
{
public:
    virtual ~Transformable() = default;

    const Matrix3x3 &getTransformMatrix() const;

    void setPosition(const SDL_FPoint &position);
    void setOriginKeepPosition(const SDL_FPoint &newOrigin);
    void setOrigin(const SDL_FPoint &origin);
    void setScale(const SDL_FPoint &scale);
    void setUniformScale(float scale);
    void setRotation(float rotation);

    void rotate(float angle);
    void move(const SDL_FPoint &offset);
    void scale(const SDL_FPoint &factor);
    void uniformeScale(float factor);
    void reset();

    const SDL_FPoint &getPosition() const;
    const SDL_FPoint &getOrigin() const;
    const SDL_FPoint &getScale() const;
    float getRotation() const;

protected:
    SDL_FPoint position_ = {0.0f, 0.0f};
    SDL_FPoint origin_ = {0.0f, 0.0f};
    SDL_FPoint scale_ = {1.0f, 1.0f};
    float rotation_ = 0.0f;

    mutable Matrix3x3 matrix_;
    mutable bool m_dirty = true;
};

} // namespace sdl3

