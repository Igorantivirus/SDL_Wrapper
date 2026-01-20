#pragma once

#include <SDLWrapper/Names.hpp>

#include <cstddef>


#include <SDLWrapper/ObjectBase/Shape.hpp>

namespace sdl3
{

class CircleShape : public Shape
{
public:
    CircleShape(float radius = 0.0f, std::size_t pointCount = 20);

    void setRadius(float radius);
    void setPointCount(std::size_t pointCount);

    const float getRadius() const;
    std::size_t getPointCount() const override;
    Vector2f getPoint(std::size_t index) const override;

private:
    float radius_;
    std::size_t pointCount_;
};

} // namespace sdl3

