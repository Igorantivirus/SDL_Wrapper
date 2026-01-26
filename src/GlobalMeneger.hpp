#pragma once

#include <cstddef>
#include <memory>
#include <unordered_map>

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

class GlobalMeneger
{
    friend class Texture;
    friend class RenderWindow;
    friend class audio::Audio;
    friend class audio::AudioDevice;

private:
    static std::size_t subscribeRenderer(std::shared_ptr<SDL_Renderer> render)
    {
        static std::size_t nextId = 0;
        renders_[nextId] = std::move(render);
        return nextId++;
    }

    static void unsubscribeRenderer(const std::size_t id)
    {
        if (id != invalidID)
            renders_.erase(id);
    }

    static std::weak_ptr<SDL_Renderer> getRenderer(const std::size_t id)
    {
        auto found = renders_.find(id);
        return found == renders_.end() ? std::weak_ptr<SDL_Renderer>{} : found->second;
    }

    static std::size_t subscribeMixer(std::shared_ptr<MIX_Mixer> mixer)
    {
        static std::size_t nextId = 0;
        mixers_[nextId] = std::move(mixer);
        return nextId++;
    }

    static void unsubscribeMixer(const std::size_t id)
    {
        if (id != invalidID)
            mixers_.erase(id);
    }

    static std::weak_ptr<MIX_Mixer> getMixer(const std::size_t id)
    {
        auto found = mixers_.find(id);
        return found == mixers_.end() ? std::weak_ptr<MIX_Mixer>{} : found->second;
    }

    inline static constexpr const std::size_t invalidID = std::size_t(-1);

private:
    inline static std::unordered_map<std::size_t, std::weak_ptr<SDL_Renderer>> renders_;
    inline static std::unordered_map<std::size_t, std::weak_ptr<MIX_Mixer>> mixers_;
};

} // namespace sdl3
