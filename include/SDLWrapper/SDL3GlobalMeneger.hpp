#pragma once

#include <cstddef>
#include <memory>
#include <mutex>
#include <unordered_map>

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3_mixer/SDL_mixer.h>

namespace sdl3
{

class Texture;
class RenderWindow;

namespace audio
{
class Audio;
class AudioDevice;
} // namespace audio

class SDL3GlobalMeneger
{
    friend class Texture;
    friend class RenderWindow;
    friend class audio::Audio;
    friend class audio::AudioDevice;

private:
    struct Storage
    {
        std::unordered_map<std::size_t, std::weak_ptr<SDL_Renderer>> renders;
        std::unordered_map<std::size_t, std::weak_ptr<MIX_Mixer>> mixers;

        std::size_t nextRenderId = 0;
        std::size_t nextMixerId = 0;
        std::mutex mtx;

        bool wantSDL = false;
        bool wantMixer = false;

        bool sdlInited = false;
        bool mixerInited = false;
    };

    inline static Storage *s_ = nullptr;

public:
    inline static constexpr std::size_t invalidID = std::size_t(-1);

    /// Инициализация SDL и/или SDL_mixer
    static bool init(bool initSDL, bool initMixer);

    /// Завершение: вызывает Quit только для тех, что реально были инициализированы.
    static void shutdown() noexcept;

    static bool isInitialized() noexcept;

private:
    static Storage *storageOrNull_NoInit() noexcept;

    static std::size_t subscribeRenderer(std::shared_ptr<SDL_Renderer> render);

    static void unsubscribeRenderer(std::size_t id) noexcept;

    static std::weak_ptr<SDL_Renderer> getRenderer(std::size_t id) noexcept;

    static std::size_t subscribeMixer(std::shared_ptr<MIX_Mixer> mixer);

    static void unsubscribeMixer(std::size_t id) noexcept;

    static std::weak_ptr<MIX_Mixer> getMixer(std::size_t id) noexcept;
};

} // namespace sdl3
