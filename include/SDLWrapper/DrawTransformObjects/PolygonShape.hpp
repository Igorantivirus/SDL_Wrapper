#pragma once

#include <cstddef>
#include <vector>

#include <SDLWrapper/ObjectBase/Shape.hpp>
#include <SDLWrapper/Names.hpp>

namespace sdl3
{

class PolygonShape : public Shape
{
public:
    PolygonShape() = default;
    
    explicit PolygonShape(std::vector<Vector2f> points);

    void setPoints(std::vector<Vector2f> points);

    const std::vector<Vector2f> &getPoints() const;

    std::size_t getPointCount() const override;

    Vector2f getPoint(const std::size_t index) const override;

private:
    std::vector<Vector2f> points_;
};

} // namespace sdl3