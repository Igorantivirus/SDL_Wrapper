#include "SDL3/SDL_pixels.h"
#include <SDLWrapper/Renders/RenderTarget.hpp>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
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
void RenderTarget::draw(const Drawable* object)
{
    object->draw(*this);
}

void RenderTarget::drawShape(const Texture *texture,
                             const Vector2f *positions, const int posCnt,
                             const Vector2f *uv, const int /*uvCnt*/,
                             const Color &color,
                             const int *indices, const int indCnt)
{
    if (!renderer_ || !posCnt)
        return;
    SDL_Texture *sdlTex = getRawTextureFromTexture(texture);
    SDL_RenderGeometryRaw(renderer_.get(), sdlTex,
                          &positions->x, sizeof(Vector2f),
                          reinterpret_cast<const SDL_FColor*>(&color), 0,
                          uv ? &uv->x : nullptr, sizeof(Vector2f),
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

Vector2f RenderTarget::getTargetCenter() const
{
    if (!renderer_)
        return Vector2f{0.0f, 0.0f};

    int logicalW = 0;
    int logicalH = 0;
    SDL_RendererLogicalPresentation logicalMode = SDL_LOGICAL_PRESENTATION_DISABLED;
    if (SDL_GetRenderLogicalPresentation(renderer_.get(), &logicalW, &logicalH, &logicalMode) &&
        logicalMode != SDL_LOGICAL_PRESENTATION_DISABLED &&
        logicalW > 0 && logicalH > 0)
    {
        // При включённом logical presentation координатная система рендера становится "логической",
        // поэтому центр должен быть в логических единицах, а не в пикселях реального output-size.
        return Vector2f{logicalW / 2.0f, logicalH / 2.0f};
    }

    int w = 0;
    int h = 0;
    if (!SDL_GetCurrentRenderOutputSize(renderer_.get(), &w, &h))
        SDL_Log("%s", SDL_GetError());

    return Vector2f{w / 2.0f, h / 2.0f};
}

void RenderTarget::clear(const Color &color)
{
    SDL_SetRenderDrawColorFloat(renderer_.get(), color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer_.get());
}

void RenderTarget::display()
{
    SDL_RenderPresent(renderer_.get());
}

void RenderTarget::setBaseViewPosition(const Vector2f &pos)
{
    view_.setCenterPosition(pos);
}

std::shared_ptr<SDL_Renderer> RenderTarget::getNativeSDLRenderer()
{
    return renderer_;
}

} // namespace sdl3
