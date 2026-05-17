#pragma once

#include <cstddef>
#include <memory>
#include <mutex>
#include <unordered_map>

#include <SDL3/SDL_render.h>

namespace sdl3
{
class RenderWindow;
class Texture;
}

namespace sdl3::detail
{

class RendererRegistry
{
    friend class sdl3::RenderWindow;
    friend class sdl3::Texture;

private:
    struct Storage
    {
        std::unordered_map<std::size_t, std::weak_ptr<SDL_Renderer>> renderers;
        std::size_t nextRendererId = 0;
        std::size_t usersCount = 0;
        std::mutex mtx;
    };

    inline static Storage* s_ = nullptr;

    inline static constexpr std::size_t invalidID = std::size_t(-1);

private:
    static Storage* storageOrNull_NoInit() noexcept;

    static std::size_t subscribeRenderer(std::shared_ptr<SDL_Renderer> renderer);
    static void unsubscribeRenderer(std::size_t id) noexcept;
    static std::weak_ptr<SDL_Renderer> getRenderer(std::size_t id) noexcept;
};

} // namespace sdl3::detail
