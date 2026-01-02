#pragma once

#include <cstddef>

#include <SDL3/SDL_rect.h>

#include <SDLWrapper/ObjectBase/Shape.hpp>

namespace sdl3
{
class RectangleShape : public Shape
{
public:
    RectangleShape(const SDL_FPoint &size = {0.f, 0.f})
        : size_{size}
    {
        updateLocalGeometry();
    }

    void setSize(const SDL_FPoint &size)
    {
        size_ = size;
        updateLocalGeometry();
    }

    const SDL_FPoint &getSize() const
    {
        return size_;
    }

    std::size_t getPointCount() const override
    {
        return 4;
    }

    SDL_FPoint getPoint(std::size_t index) const override
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

private:
    SDL_FPoint size_{0.f, 0.f};
};
} // namespace sdl3
