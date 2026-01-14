#pragma once

#include <cstddef>
#include <memory>
#include <unordered_map>

#include <SDL3/SDL_render.h>

namespace sdl3
{

class Texture;
class RenderWindow;

class RenderMeneger
{
    friend class Texture;
    friend class RenderWindow;

private:
    static std::size_t subscribeRenderer(std::shared_ptr<SDL_Renderer> render);
    static void unsubscribeRenderer(std::size_t id);
    static std::weak_ptr<SDL_Renderer> getRenderer(std::size_t id);

    inline static constexpr const std::size_t invalidID = std::size_t(-1);

private:
    static std::unordered_map<std::size_t, std::weak_ptr<SDL_Renderer>> renders_;
};

} // namespace sdl3
