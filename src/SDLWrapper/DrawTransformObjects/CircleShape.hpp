#pragma once

#include <cstddef>
#include <cmath>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_stdinc.h>
#include <SDLWrapper/ObjectBase/Shape.hpp>

namespace sdl3
{
class CircleShape : public Shape
{
public:
    CircleShape(float radius = 0.0f, std::size_t pointCount = 20)
        : radius_(radius), pointCount_(pointCount)
    {
        updateLocalGeometry();
    }

    void setRadius(float radius)
    {
        radius_ = radius;
        updateLocalGeometry();
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
            radius_ * std::cos(angle),
            radius_ * std::sin(angle)};
    }

private:
    float radius_;
    std::size_t pointCount_;
};

} // namespace sdl3
