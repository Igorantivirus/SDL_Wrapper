#pragma once

#include <cstddef>
#include <memory>
#include <mutex>
#include <unordered_map>

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

namespace sdl3
{

class Texture;
class RenderWindow;

class SDL3GlobalMeneger
{
    friend class Texture;
    friend class RenderWindow;

private:
    struct Storage
    {
        std::unordered_map<std::size_t, std::weak_ptr<SDL_Renderer>> renders;

        std::size_t nextRenderId = 0;
        std::mutex mtx;

        bool sdlInited = false;
        SDL_InitFlags initFlags = 0;
    };

    inline static Storage *s_ = nullptr;

public:
    inline static constexpr std::size_t invalidID = std::size_t(-1);

    static bool init(SDL_InitFlags flags = SDL_INIT_VIDEO);
    static void shutdown() noexcept;

    static bool isInitialized() noexcept;

private:
    static Storage *storageOrNull_NoInit() noexcept;

    static std::size_t subscribeRenderer(std::shared_ptr<SDL_Renderer> render);

    static void unsubscribeRenderer(std::size_t id) noexcept;

    static std::weak_ptr<SDL_Renderer> getRenderer(std::size_t id) noexcept;
};

} // namespace sdl3