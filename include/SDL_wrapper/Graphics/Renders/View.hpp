#pragma once

#include <SDL_wrapper/Graphics/Export.hpp>

#include <SDL_wrapper/Core/Names.hpp>
#include <SDL_wrapper/Core/Math/Matrix3x3.hpp>

namespace sdl3
{

class SDL_WRAPPER_GRAPHICS_EXPORT View
{
public:
    View() = default;

    void setCenterPosition(const Vector2f &pos);
    const Vector2f &getCenterPosition() const;

    void setZoom(const Vector2f &zoom);
    void setUniformScale(float zoom);
    const Vector2f &getZoom() const;

    void setAngle(float degrees);
    float getAngle() const;
    void rotate(float angle);

    const Matrix3x3<float> &getTransformMatrix() const;

    void reset();

    bool operator==(const View &other);
    bool operator!=(const View &other);

private:
    Vector2f center_{0.0f, 0.0f};
    Vector2f zoom_ = {1.f, 1.f};
    float angle_ = 0.f;

    mutable Matrix3x3<float> matrix_;
    mutable bool m_dirty = true;
};

} // namespace sdl3
