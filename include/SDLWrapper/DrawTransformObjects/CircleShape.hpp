#pragma once

#include <cstddef>

#include <SDL3/SDL_rect.h>

#include <SDLWrapper/ObjectBase/Shape.hpp>

namespace sdl3
{

class CircleShape : public Shape
{
public:
    CircleShape(float radius = 0.0f, std::size_t pointCount = 20);

    void setRadius(float radius);
    void setPointCount(std::size_t pointCount);

    std::size_t getPointCount() const override;
    SDL_FPoint getPoint(std::size_t index) const override;

private:
    float radius_;
    std::size_t pointCount_;
};

} // namespace sdl3

