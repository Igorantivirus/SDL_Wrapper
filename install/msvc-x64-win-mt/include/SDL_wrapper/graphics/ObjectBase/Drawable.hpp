#pragma once

#include <SDL_wrapper/Graphics/Export.hpp>

namespace sdl3
{

class RenderTarget;

class SDL_WRAPPER_GRAPHICS_EXPORT Drawable
{
public:

    friend class RenderTarget;

public:
    virtual ~Drawable() = default;

protected:
    virtual void draw(RenderTarget &target) const = 0;

protected:

    mutable unsigned viewID_ = static_cast<unsigned>(-1);

};

} // namespace sdl3