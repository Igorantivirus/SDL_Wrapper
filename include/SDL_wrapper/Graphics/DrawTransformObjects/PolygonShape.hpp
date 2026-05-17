#pragma once

#include <SDL_wrapper/Graphics/Export.hpp>

#include <cstddef>
#include <vector>

#include <SDL_wrapper/Graphics/ObjectBase/Shape.hpp>
#include <SDL_wrapper/Core/Names.hpp>

namespace sdl3
{

class SDL_WRAPPER_GRAPHICS_EXPORT PolygonShape : public Shape
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