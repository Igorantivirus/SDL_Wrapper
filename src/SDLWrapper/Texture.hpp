#pragma once

#include <SDL3/SDL_log.h>
#include <memory>

#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>

#include <SDLWrapper/Renders/RenderMeneger.hpp>
// #include <string_view>

namespace sdl3
{

class Texture
{
public:
    Texture(const std::size_t windowID = 0) : windowID_(windowID)
    {
    }

    bool loadFromFile(const char *fileName)
    {
        clear();
        std::shared_ptr<SDL_Renderer> rendererS = RenderMeneger::getRenderer(windowID_).lock();
        if (!rendererS)
        {
            SDL_Log("There is no renderer to open the texture Texture.hpp:27\n");
            return false;
        }
        SDL_Renderer *renderer = rendererS.get();

        SDL_Texture *texture = IMG_LoadTexture(renderer, fileName);
        if (!texture)
        {
            SDL_Log("Failed to load texture Texture.hpp:35 from file '%s': %s", fileName, SDL_GetError());
            return false;
        }
        texture_.reset(texture, SDL_DestroyTexture);
        updateSize();
        return true;
    }

    void clear()
    {
        size_ = {};
        texture_.reset();
    }

    const std::weak_ptr<const SDL_Texture> getSDLTexture() const
    {
        return texture_;
    }
    std::weak_ptr<SDL_Texture> getSDLTexture()
    {
        return texture_;
    }
    const SDL_Point &getSize() const
    {
        return size_;
    }

private:
    std::shared_ptr<SDL_Texture> texture_ = nullptr;
    SDL_Point size_ = {};

    std::size_t windowID_ = std::size_t(-1);

private:
    void updateSize()
    {
        if (!texture_)
        {
            size_ = {};
            return;
        }
        auto messageTexProps = SDL_GetTextureProperties(texture_.get());
        size_.x = float(SDL_GetNumberProperty(messageTexProps, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0));
        size_.y = float(SDL_GetNumberProperty(messageTexProps, SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0));
    }
};

} // namespace sdl3