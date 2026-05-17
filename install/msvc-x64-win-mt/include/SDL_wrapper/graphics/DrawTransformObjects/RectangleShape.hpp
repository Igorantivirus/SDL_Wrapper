#pragma once

#include <SDL_wrapper/Graphics/Export.hpp>

#include <SDL_wrapper/Core/Names.hpp>

#include <cstddef>


#include <SDL_wrapper/Graphics/ObjectBase/Shape.hpp>

namespace sdl3
{

class SDL_WRAPPER_GRAPHICS_EXPORT RectangleShape : public Shape
{
public:
    explicit RectangleShape(const Vector2f &size = {0.f, 0.f});

    void setSize(const Vector2f &size);
    const Vector2f &getSize() const;

    std::size_t getPointCount() const override;
    Vector2f getPoint(std::size_t index) const override;

private:
    Vector2f size_{0.f, 0.f};
};

} // namespace sdl3

