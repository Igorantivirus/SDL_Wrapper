#include <SDLWrapper/DrawTransformObjects/PolygonShape.hpp>

namespace sdl3
{

PolygonShape::PolygonShape(std::vector<Vector2f> points) : points_(std::move(points))
{
    updateLocalGeometry();
}

void PolygonShape::setPoints(std::vector<Vector2f> points)
{
    points_ = std::move(points);
    updateLocalGeometry();
}

const std::vector<Vector2f> &PolygonShape::getPoints() const
{
    return points_;
}

std::size_t PolygonShape::getPointCount() const
{
    return points_.size();
}

Vector2f PolygonShape::getPoint(const std::size_t index) const
{
    return points_[index];
}

} // namespace sdl3
