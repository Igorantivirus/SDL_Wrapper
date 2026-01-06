#pragma once

#include <SDLWrapper/Names.hpp>

#include <cstddef>


#include <SDLWrapper/ObjectBase/Shape.hpp>

namespace sdl3
{

class RectangleShape : public Shape
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

