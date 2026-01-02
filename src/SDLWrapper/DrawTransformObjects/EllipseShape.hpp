#pragma once

#include <basetsd.h>
#include <cmath>
#include <cstddef>

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_stdinc.h>

#include <SDLWrapper/ObjectBase/Shape.hpp>

namespace sdl3
{
class EllipseShape : public Shape
{
public:
    EllipseShape(const SDL_FPoint &radii = {0.0f, 0.0f}, std::size_t pointCount = 40)
        : radii_{radii}, pointCount_(pointCount)
    {
        updateLocalGeometry();
    }

    void setRadii(const SDL_FPoint &radii)
    {
        radii_ = radii;
        updateLocalGeometry();
    }

    const SDL_FPoint &getRadii() const
    {
        return radii_;
    }

    void setPointCount(std::size_t pointCount)
    {
        pointCount_ = pointCount;
        updateLocalGeometry();
    }

    std::size_t getPointCount() const override
    {
        return pointCount_;
    }

    SDL_FPoint getPoint(std::size_t index) const override
    {
        const float angle = static_cast<float>(index) * 2.0f * SDL_PI_F / static_cast<float>(pointCount_);
        return {
            radii_.x * std::cos(angle),
            radii_.y * std::sin(angle)};
    }

private:
    SDL_FPoint radii_{0.0f, 0.0f};
    std::size_t pointCount_;
};
} // namespace sdl3
