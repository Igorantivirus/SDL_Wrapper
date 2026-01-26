#pragma once

#include <list>
#include <memory>
#include <vector>

#include "Audio.hpp"

namespace sdl3::audio
{

class AudioDevice
{
private:
    // Гарантия, что аудио будет жить, пока воспроизводится
    struct AudioPair
    {
        std::shared_ptr<MIX_Track> track;
        const Audio *audio;
    };

public:
    AudioDevice() = default;

    bool initTracks(const std::size_t tracksCount);

    bool playSound(const Audio &audio, bool replay = false);

    std::size_t update();

    std::size_t getFreeTracksCount() const;
    std::size_t getUsedTracksCount() const;

    std::weak_ptr<const MIX_Mixer> getSDMixer() const;
    std::weak_ptr<MIX_Mixer> getSDMixer();

    const std::size_t getDeviceID() const;

private:
    std::shared_ptr<MIX_Mixer> mixer_;
    std::vector<std::shared_ptr<MIX_Track>> freeTracks_;
    std::list<AudioPair> usedTracks_;

    std::size_t deviceID_ = std::size_t(-1);

private:
    void subscribe();
    void unsubscribe();
};
} // namespace sdl3::audio