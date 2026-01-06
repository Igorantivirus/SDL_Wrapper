#include <SDLWrapper/DrawTransformObjects/EllipseShape.hpp>

#include <cmath>

#include <SDL3/SDL_stdinc.h>

namespace sdl3
{

EllipseShape::EllipseShape(const Vector2f &radii, const std::size_t pointCount)
    : radii_{radii}, pointCount_(pointCount)
{
    updateLocalGeometry();
}

void EllipseShape::setRadii(const Vector2f &radii)
{
    radii_ = radii;
    updateLocalGeometry();
}

const Vector2f &EllipseShape::getRadii() const
{
    return radii_;
}

void EllipseShape::setPointCount(const std::size_t pointCount)
{
    pointCount_ = pointCount;
    updateLocalGeometry();
}

std::size_t EllipseShape::getPointCount() const
{
    return pointCount_;
}

Vector2f EllipseShape::getPoint(const std::size_t index) const
{
    const float angle = static_cast<float>(index) * 2.0f * SDL_PI_F / static_cast<float>(pointCount_);
    return {
        radii_.x * std::cos(angle),
        radii_.y * std::sin(angle)};
}

} // namespace sdl3

