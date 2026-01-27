#include <SDLWrapper/SDL3GlobalMeneger.hpp>
#include <SDLWrapper/Texture.hpp>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>

struct TextureDeleter
{
    void operator()(SDL_Texture *texture) const noexcept
    {
        SDL_DestroyTexture(texture);
    }
};

namespace sdl3
{

Texture::Texture(const std::size_t windowID) : windowID_(windowID)
{
}

bool Texture::loadFromFile(const char *fileName)
{
    clear();
    std::shared_ptr<SDL_Renderer> rendererS = SDL3GlobalMeneger::getRenderer(windowID_).lock();
    if (!rendererS)
    {
        SDL_Log("There is no renderer to open the texture");
        return false;
    }
    SDL_Renderer *renderer = rendererS.get();

    SDL_Texture *texture = IMG_LoadTexture(renderer, fileName);
    if (!texture)
    {
        SDL_Log("%s", SDL_GetError());
        return false;
    }
    texture_.reset(texture, TextureDeleter{});
    updateSize();
    return true;
}

void Texture::clear()
{
    size_ = {};
    texture_.reset();
}

std::weak_ptr<const SDL_Texture> Texture::getSDLTexture() const
{
    return texture_;
}

std::weak_ptr<SDL_Texture> Texture::getSDLTexture()
{
    return texture_;
}

const Vector2i &Texture::getSize() const
{
    return size_;
}

void Texture::updateSize()
{
    if (!texture_)
    {
        size_ = {};
        return;
    }
    auto messageTexProps = SDL_GetTextureProperties(texture_.get());
    size_.x = static_cast<int>(SDL_GetNumberProperty(messageTexProps, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0));
    size_.y = static_cast<int>(SDL_GetNumberProperty(messageTexProps, SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0));
}

} // namespace sdl3