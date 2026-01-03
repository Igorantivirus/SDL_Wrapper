#include <SDLWrapper/Renders/RenderTarget.hpp>

#include <SDL3/SDL_render.h>

#include <SDLWrapper/ObjectBase/Drawable.hpp>
#include <SDLWrapper/Texture.hpp>

SDL_Texture *getRawTextureFromTexture(const sdl3::Texture *texture)
{
    if (texture)
        return const_cast<SDL_Texture *>(texture->getSDLTexture().lock().get());
    return nullptr;
}

namespace sdl3
{

void RenderTarget::draw(const Drawable &object)
{
    object.draw(*this);
}

void RenderTarget::drawShape(const Texture *texture,
                             const SDL_FPoint *positions, const int posCnt,
                             const SDL_FPoint *uv, const int /*uvCnt*/,
                             const SDL_FColor &color,
                             const int *indices, const int indCnt)
{
    if (!renderer_ || !posCnt)
        return;
    SDL_Texture *sdlTex = getRawTextureFromTexture(texture);
    SDL_RenderGeometryRaw(renderer_.get(), sdlTex,
                          &positions->x, sizeof(SDL_FPoint),
                          &color, 0,
                          uv ? &uv->x : nullptr, sizeof(SDL_FPoint),
                          posCnt,
                          indices, indCnt, sizeof(int));
}

const View &RenderTarget::getView() const
{
    return view_;
}

void RenderTarget::setView(const View &view)
{
    if (view_ != view)
    {
        view_ = view;
        ++viewId_;
    }
}

unsigned RenderTarget::getViewId() const
{
    return viewId_;
}

SDL_FPoint RenderTarget::getTargetCenter() const
{
    return SDL_FPoint{targetSize_.x / 2.0f, targetSize_.y / 2.0f};
}

void RenderTarget::clear(const SDL_Color &color)
{
    SDL_SetRenderDrawColor(renderer_.get(), color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer_.get());
}

void RenderTarget::display()
{
    SDL_RenderPresent(renderer_.get());
}

void RenderTarget::setBaseViewPosition(const SDL_FPoint &pos)
{
    view_.setCenterPosition(pos);
}

} // namespace sdl3
