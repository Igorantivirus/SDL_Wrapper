#pragma once

#include <cstddef>

#include <SDL3/SDL_rect.h>

#include <SDLWrapper/ObjectBase/Shape.hpp>

namespace sdl3
{

class EllipseShape : public Shape
{
public:
    EllipseShape(const SDL_FPoint &radii = {0.0f, 0.0f}, std::size_t pointCount = 40);

    void setRadii(const SDL_FPoint &radii);
    const SDL_FPoint &getRadii() const;

    void setPointCount(std::size_t pointCount);

    std::size_t getPointCount() const override;
    SDL_FPoint getPoint(std::size_t index) const override;

private:
    SDL_FPoint radii_{0.0f, 0.0f};
    std::size_t pointCount_;
};

} // namespace sdl3

