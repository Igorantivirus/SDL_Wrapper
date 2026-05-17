#pragma once

#include <cstddef>
#include <memory>
#include <mutex>
#include <unordered_map>

#include <SDL3_mixer/SDL_mixer.h>

namespace sdl3::mixer
{
class Audio;
class AudioDevice;
}

namespace sdl3::mixer::detail
{

class MixerRegistry
{
    friend class sdl3::mixer::Audio;
    friend class sdl3::mixer::AudioDevice;

private:
    struct Storage
    {
        std::unordered_map<std::size_t, std::weak_ptr<MIX_Mixer>> mixers;
        std::size_t nextMixerId = 0;
        std::size_t usersCount = 0;
        std::mutex mtx;
    };

    inline static Storage* s_ = nullptr;

    inline static constexpr std::size_t invalidID = std::size_t(-1);

private:
    static Storage* storageOrNull_NoInit() noexcept;

    static std::size_t subscribeMixer(std::shared_ptr<MIX_Mixer> mixer);
    static void unsubscribeMixer(std::size_t id) noexcept;
    static std::weak_ptr<MIX_Mixer> getMixer(std::size_t id) noexcept;
};

} // namespace sdl3::mixer::detail
