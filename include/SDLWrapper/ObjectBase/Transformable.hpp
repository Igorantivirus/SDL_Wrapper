#pragma once

#include <SDLWrapper/Names.hpp>
#include <SDLWrapper/Math/Matrix3x3.hpp>

namespace sdl3
{

class Transformable
{
public:
    virtual ~Transformable() = default;

    const Matrix3x3<float> &getTransformMatrix() const;

    void setPosition(const Vector2f &position);
    void setOriginKeepPosition(const Vector2f &newOrigin);
    void setOrigin(const Vector2f &origin);
    void setScale(const Vector2f &scale);
    void setUniformScale(float scale);
    void setRotation(float rotation);

    void rotate(float angle);
    void move(const Vector2f &offset);
    void scale(const Vector2f &factor);
    void uniformeScale(float factor);
    void reset();

    const Vector2f &getPosition() const;
    const Vector2f &getOrigin() const;
    const Vector2f &getScale() const;
    float getRotation() const;

protected:
    Vector2f position_ = {0.0f, 0.0f};
    Vector2f origin_ = {0.0f, 0.0f};
    Vector2f scale_ = {1.0f, 1.0f};
    float rotation_ = 0.0f;

    mutable Matrix3x3<float> matrix_;
    mutable bool m_dirty = true;
};

} // namespace sdl3

