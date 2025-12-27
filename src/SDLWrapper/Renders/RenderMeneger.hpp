#pragma once

#include <SDL3/SDL_render.h>

#include <memory>
#include <unordered_map>

namespace sdl3
{

class Texture;
class RenderWindow;

class RenderMeneger
{
    friend class Texture;
    friend class RenderWindow;

private:
    static std::size_t subscribeRenderer(std::shared_ptr<SDL_Renderer> render)
    {
        static std::size_t nextId = 0;
        renders_[nextId] = render;
        return nextId++;
    }
    static void unsubscribeRenderer(const std::size_t id)
    {
        renders_.erase(id);
    }
    static std::weak_ptr<SDL_Renderer> getRenderer(const std::size_t id)
    {
        auto found = renders_.find(id);
        return found == renders_.end() ? std::weak_ptr<SDL_Renderer>{} : found->second;
    }

private:
    static std::unordered_map<std::size_t, std::weak_ptr<SDL_Renderer>> renders_;
};

#ifndef RENDER_MENEGER_RENDERERS
#define RENDER_MENEGER_RENDERERS
std::unordered_map<std::size_t, std::weak_ptr<SDL_Renderer>> RenderMeneger::renders_{};
#endif


} // namespace sdl3