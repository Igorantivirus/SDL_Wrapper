#pragma once

#include <SDL_wrapper/Graphics/Export.hpp>

#include <SDL_wrapper/Core/Names.hpp>

#include <cstddef>


#include <SDL_wrapper/Graphics/ObjectBase/Shape.hpp>

namespace sdl3
{

class SDL_WRAPPER_GRAPHICS_EXPORT EllipseShape : public Shape
{
public:
    EllipseShape(const Vector2f &radii = {0.0f, 0.0f}, std::size_t pointCount = 40);

    void setRadii(const Vector2f &radii);
    const Vector2f &getRadii() const;

    void setPointCount(std::size_t pointCount);

    std::size_t getPointCount() const override;
    Vector2f getPoint(std::size_t index) const override;

private:
    Vector2f radii_{0.0f, 0.0f};
    std::size_t pointCount_;
};

} // namespace sdl3

