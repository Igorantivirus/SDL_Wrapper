#pragma once

#include <SDL_wrapper/Graphics/Export.hpp>

#include <memory>

#include <SDL3/SDL_render.h>

#include <SDL_wrapper/Core/Math/Colors.hpp>
#include <SDL_wrapper/Core/Names.hpp>
#include <SDL_wrapper/Graphics/Renders/View.hpp>
#include <SDL_wrapper/Graphics/Texture.hpp>


namespace sdl3
{

class Drawable;
class Texture;

class SDL_WRAPPER_GRAPHICS_EXPORT RenderTarget
{
public:
    virtual ~RenderTarget() = default;

    void draw(const Drawable &object);
    void draw(const Drawable *object);

    void drawShape(const Texture *texture,
                   const Vector2f *positions, int posCnt,
                   const Vector2f *uv, int uvCnt,
                   const Color &color,
                   const int *indices, int indCnt);

    const View &getView() const;
    void setView(const View &view);
    unsigned getViewId() const;
    Vector2f getTargetCenter() const;

    void clear(const Color &color = Colors::Black);
    void display();

    std::shared_ptr<SDL_Renderer> getNativeSDLRenderer();

protected:
    std::shared_ptr<SDL_Renderer> renderer_;
    View view_;

    unsigned viewId_ = 1;

protected:
    void setBaseViewPosition(const Vector2f &pos);
};

} // namespace sdl3
