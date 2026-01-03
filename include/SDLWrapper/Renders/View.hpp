#pragma once

#include <SDL3/SDL_rect.h>

#include <SDLWrapper/Math/Matrix3x3.hpp>

namespace sdl3
{

class View
{
public:
    View() = default;

    void setCenterPosition(const SDL_FPoint &pos);
    const SDL_FPoint &getCenterPosition() const;

    void setZoom(const SDL_FPoint &zoom);
    void setUniformScale(float zoom);
    const SDL_FPoint &getZoom() const;

    void setAngle(float degrees);
    float getAngle() const;
    void rotate(float angle);

    const Matrix3x3 &getTransformMatrix() const;

    void reset();

    bool operator==(const View &other);
    bool operator!=(const View &other);

private:
    SDL_FPoint center_{0.0f, 0.0f};
    SDL_FPoint zoom_ = {1.f, 1.f};
    float angle_ = 0.f;

    mutable Matrix3x3 matrix_;
    mutable bool m_dirty = true;
};

} // namespace sdl3