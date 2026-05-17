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

protected:

    mutable unsigned viewID_ = static_cast<unsigned>(-1);

};

} // namespace sdl3