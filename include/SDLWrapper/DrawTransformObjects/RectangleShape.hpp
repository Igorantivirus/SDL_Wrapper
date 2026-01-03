#pragma once

#include <cstddef>

#include <SDL3/SDL_rect.h>

#include <SDLWrapper/ObjectBase/Shape.hpp>

namespace sdl3
{

class RectangleShape : public Shape
{
public:
    explicit RectangleShape(const SDL_FPoint &size = {0.f, 0.f});

    void setSize(const SDL_FPoint &size);
    const SDL_FPoint &getSize() const;

    std::size_t getPointCount() const override;
    SDL_FPoint getPoint(std::size_t index) const override;

private:
    SDL_FPoint size_{0.f, 0.f};
};

} // namespace sdl3

