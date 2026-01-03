#pragma once

namespace sdl3
{

class RenderTarget;

class Drawable
{
public:

    friend class RenderTarget;

public:
    virtual ~Drawable() = default;

protected:
    virtual void draw(RenderTarget &target) const = 0;
};

} // namespace sdl3