#include <SDLWrapper/DrawTransformObjects/RectangleShape.hpp>

namespace sdl3
{

RectangleShape::RectangleShape(const SDL_FPoint &size)
    : size_{size}
{
    updateLocalGeometry();
}

void RectangleShape::setSize(const SDL_FPoint &size)
{
    size_ = size;
    updateLocalGeometry();
}

const SDL_FPoint &RectangleShape::getSize() const
{
    return size_;
}

std::size_t RectangleShape::getPointCount() const
{
    return 4;
}

SDL_FPoint RectangleShape::getPoint(const std::size_t index) const
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

