#pragma once

#include <cstddef>
#include <memory>

#include <SDL3/SDL_render.h>

#include <SDLWrapper/Names.hpp>

namespace sdl3
{

class Texture
{
public:
    explicit Texture(std::size_t windowID = 0);

    bool loadFromFile(const char *fileName);
    void clear();

    std::weak_ptr<const SDL_Texture> getSDLTexture() const;
    std::weak_ptr<SDL_Texture> getSDLTexture();

    const Vector2i &getSize() const;

private:

    std::shared_ptr<SDL_Texture> texture_ = nullptr;
    Vector2i size_ = {};

    std::size_t windowID_ = std::size_t(-1);

private:
    void updateSize();
};

} // namespace sdl3

