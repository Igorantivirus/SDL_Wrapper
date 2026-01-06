#include <SDLWrapper/DrawTransformObjects/CircleShape.hpp>

#include <cmath>

#include <SDL3/SDL_stdinc.h>

namespace sdl3
{

CircleShape::CircleShape(const float radius, const std::size_t pointCount)
    : radius_(radius), pointCount_(pointCount)
{
    updateLocalGeometry();
}

void CircleShape::setRadius(const float radius)
{
    radius_ = radius;
    updateLocalGeometry();
}

void CircleShape::setPointCount(const std::size_t pointCount)
{
    pointCount_ = pointCount;
    updateLocalGeometry();
}

std::size_t CircleShape::getPointCount() const
{
    return pointCount_;
}

Vector2f CircleShape::getPoint(const std::size_t index) const
{
    const float angle = static_cast<float>(index) * 2.0f * SDL_PI_F / static_cast<float>(pointCount_);
    return {
        radius_ * std::cos(angle),
        radius_ * std::sin(angle)};
}

} // namespace sdl3

