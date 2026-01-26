#pragma once

#include <cstddef>
#include <memory>

#include <SDL3_mixer/SDL_mixer.h>

namespace sdl3::audio
{

class Audio
{
public:
    explicit Audio(std::size_t deviceID = 0);
    Audio(const Audio &other) = delete;
    Audio(Audio &&other) = default;
    Audio &operator=(const Audio &other) = delete;
    Audio &operator=(Audio &&other) = default;

    void stop();

    bool loadFromFile(const char* path, bool predecode = true);

    std::weak_ptr<const MIX_Audio> getSDLAudio() const;
    std::weak_ptr<MIX_Audio> getSDLAudio();

    operator bool() const;

    bool isRunnig() const;

private:
    std::shared_ptr<MIX_Audio> audio_ = nullptr;
    bool isRunning_ = false;

    std::size_t deviceID_ = std::size_t(-1);
};

} // namespace audio