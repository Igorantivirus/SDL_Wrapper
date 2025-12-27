#pragma once

#include <SDL3/SDL_stdinc.h>
#include <SDLWrapper/ObjectBase/Shape.hpp>

namespace sdl3
{
class CircleShape : public Shape
{
public:
    CircleShape(float radius = 0, size_t pointCount = 30)
        : radius_(radius), pointCount_(pointCount)
    {
        update();
    }

    void setRadius(float radius)
    {
        radius_ = radius;
        update();
    }

    size_t getPointCount() const override
    {
        return pointCount_;
    }

    SDL_FPoint getPoint(size_t index) const override
    {
        float angle = index * 2 * SDL_PI_F / pointCount_;
        return {
            radius_ * std::cos(angle),
            radius_ * std::sin(angle)};
    }

private:
    float radius_;
    size_t pointCount_;
};

} // namespace sdl3