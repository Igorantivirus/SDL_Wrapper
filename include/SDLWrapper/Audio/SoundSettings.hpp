#pragma once

#include <SDLWrapper/Names.hpp>

#include <SDL3/SDL_properties.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <memory>

namespace sdl3::audio
{

class Sound;
class AudioDevice;

struct PlayProperties
{
public:
    int loopCount = 0;       // MIX_PROP_PLAY_LOOPS_NUMBER
    float startMS = 0.f;     // MIX_PROP_PLAY_START_MILLISECOND_NUMBER
    float loopPointMS = 0.f; // MIX_PROP_PLAY_LOOP_START_MILLISECOND_NUMBER
    float maxMS = -1.f;      // MIX_PROP_PLAY_MAX_MILLISECONDS_NUMBER (-1 = Р±РµР· Р»РёРјРёС‚Р°)
    float silenceMS = 0.f;   // MIX_PROP_PLAY_APPEND_SILENCE_MILLISECONDS_NUMBER

    // (РѕРїС†РёРѕРЅР°Р»СЊРЅРѕ вЂ” Сѓ С‚РµР±СЏ РЅРµ Р±С‹Р»Рѕ, РЅРѕ SDL3_mixer СЌС‚Рѕ РїРѕРґРґРµСЂР¶РёРІР°РµС‚)
    float fadeInMS = 0.f;        // MIX_PROP_PLAY_FADE_IN_MILLISECONDS_NUMBER
    float fadeInStartGain = 0.f; // MIX_PROP_PLAY_FADE_IN_START_GAIN_FLOAT (0..1)

public:
    static PlayProperties getDefaultProperties();
    static SDL_PropertiesID generateProperties(const PlayProperties &p);
};

struct LiveControls
{
    friend class Sound;
    friend class AudioDevice;

    float volume = 1.f;         // РїСЂРёРјРµРЅСЏР№ С‡РµСЂРµР· MIX_SetTrackGain()
    float frequencyRatio = 1.f; // MIX_SetTrackFrequencyRatio()
    Vector3f position = {};     // MIX_SetTrack3DPosition()
    Vector2f stereoGains = {1.f, 1.f};  // MIX_SetTrackStereo()

    void setControlsToTrack(std::shared_ptr<MIX_Track> track) const;

private:
    mutable unsigned version_ = 0;
};

} // namespace sdl3::audio
