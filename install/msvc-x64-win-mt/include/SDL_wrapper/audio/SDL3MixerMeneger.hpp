#pragma once

#include <cstddef>
#include <memory>
#include <mutex>
#include <unordered_map>

#include <SDL3_mixer/SDL_mixer.h>

namespace sdl3::audio
{

class Audio;
class AudioDevice;

class SDL3MixerMeneger
{
    friend class Audio;
    friend class AudioDevice;

private:
    struct Storage
    {
        std::unordered_map<std::size_t, std::weak_ptr<MIX_Mixer>> mixers;

        std::size_t nextMixerId = 0;
        std::mutex mtx;

        bool mixerInited = false;
    };

    inline static Storage *s_ = nullptr;

public:
    inline static constexpr std::size_t invalidID = std::size_t(-1);

    static bool init(SDL_InitFlags sdlFlags = SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    static void shutdown() noexcept;

    static bool isInitialized() noexcept;

private:
    static Storage *storageOrNull_NoInit() noexcept;

    static std::size_t subscribeMixer(std::shared_ptr<MIX_Mixer> mixer);

    static void unsubscribeMixer(std::size_t id) noexcept;

    static std::weak_ptr<MIX_Mixer> getMixer(std::size_t id) noexcept;
};

} // namespace sdl3::audio