#include <SDLWrapper/DrawTransformObjects/RectangleShape.hpp>

namespace sdl3
{

RectangleShape::RectangleShape(const Vector2f &size)
    : size_{size}
{
    updateLocalGeometry();
}

void RectangleShape::setSize(const Vector2f &size)
{
    size_ = size;
    updateLocalGeometry();
}

const Vector2f &RectangleShape::getSize() const
{
    return size_;
}

std::size_t RectangleShape::getPointCount() const
{
    return 4;
}

Vector2f RectangleShape::getPoint(const std::size_t index) const
{
    switch (index)
    {
    case 0:
        return {0.f, 0.f};
    case 1:
        return {size_.x, 0.f};
    case 2:
        return {size_.x, size_.y};
    default:
        return {0.f, size_.y};
    }
}

} // namespace sdl3

