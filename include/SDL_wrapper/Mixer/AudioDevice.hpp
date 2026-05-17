#pragma once

#include <SDL_wrapper/Mixer/Export.hpp>

#include <list>
#include <memory>
#include <vector>

#include <SDL3_mixer/SDL_mixer.h>

#include <SDL_wrapper/Mixer/Sound.hpp>
#include <SDL_wrapper/Mixer/SoundSettings.hpp>

namespace sdl3::mixer
{

class SDL_WRAPPER_MIXER_EXPORT AudioDevice
{
private:
    // Р“Р°СЂР°РЅС‚РёСЏ, С‡С‚Рѕ Р°СѓРґРёРѕ Р±СѓРґРµС‚ Р¶РёС‚СЊ, РїРѕРєР° РІРѕСЃРїСЂРѕРёР·РІРѕРґРёС‚СЃСЏ
    struct SoundPair
    {
        std::shared_ptr<MIX_Track> track;
        const Sound *sound;
        bool appliedPaused = false; // РєР°РєРѕРµ СЃРѕСЃС‚РѕСЏРЅРёРµ РїР°СѓР·С‹ РјС‹ СѓР¶Рµ РїСЂРёРјРµРЅРёР»Рё Рє С‚СЂРµРєСѓ
        unsigned lastSeenVersion = 0;

        bool needDelete();
    };

public:
    AudioDevice() = default;
    AudioDevice(const std::size_t tracksCount);
    ~AudioDevice();

    void close();

    bool initTracks(const std::size_t tracksCount);

    bool playSound(const Sound &sound, const PlayProperties properties = PlayProperties::getDefaultProperties());

    std::size_t update();

    std::size_t getFreeTracksCount() const;
    std::size_t getUsedTracksCount() const;

    std::weak_ptr<const MIX_Mixer> getSDMixer() const;
    std::weak_ptr<MIX_Mixer> getSDMixer();

    const std::size_t getDeviceID() const;

    void setVolumeLevel(const float value);

    void setStopedAll(const bool isStoped);
    bool isStopedAll() const;

private:
    bool isStoped_ = false;

    std::shared_ptr<MIX_Mixer> mixer_;
    std::vector<std::shared_ptr<MIX_Track>> freeTracks_;
    std::list<SoundPair> usedTracks_;

    std::size_t deviceID_ = std::size_t(-1);

private:
    void subscribe();
    void unsubscribe();
};
} // namespace sdl3::mixer

