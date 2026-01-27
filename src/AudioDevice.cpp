#include "SDL3_mixer/SDL_mixer.h"
#include <SDLWrapper/Audio/AudioDevice.hpp>

#include <SDLWrapper/SDL3GlobalMeneger.hpp>

#include <SDL3/SDL_error.h>

// loops: 0 = no loop, 1 = loop once, -1 = infinite
static SDL_PropertiesID makePlayOptions(int loops)
{
    if (loops == 0)
    {
        return 0; // "defaults for everything" :contentReference[oaicite:1]{index=1}
    }

    SDL_PropertiesID props = SDL_CreateProperties();
    if (props == 0)
    {
        // SDL_GetError() если хочешь залогировать
        return 0;
    }

    // MIX_PROP_PLAY_LOOPS_NUMBER: -1 infinite, 0 none, 1+ finite :contentReference[oaicite:2]{index=2}
    SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, loops);
    return props;
}

static void destroyPlayOptions(SDL_PropertiesID props)
{
    if (props != 0)
    {
        SDL_DestroyProperties(props);
    }
}

namespace sdl3::audio
{

void AudioDevice::subscribe()
{
    deviceID_ = SDL3GlobalMeneger::subscribeMixer(mixer_);
}

void AudioDevice::unsubscribe()
{
    SDL3GlobalMeneger::unsubscribeMixer(deviceID_);
    deviceID_ = SDL3GlobalMeneger::invalidID;
}

const std::size_t AudioDevice::getDeviceID() const
{
    return deviceID_;
}

AudioDevice::AudioDevice(const std::size_t tracksCount)
{
    void(initTracks(tracksCount));
}
AudioDevice::~AudioDevice()
{
    close();
}

void AudioDevice::close()
{
    unsubscribe();

    usedTracks_.clear();
    freeTracks_.clear();

    mixer_.reset();
}

bool AudioDevice::initTracks(const std::size_t tracksCount)
{
    close();
    MIX_Mixer *rawMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (!rawMixer)
    {
        SDL_Log("%s\n", SDL_GetError());
        return false;
    }
    mixer_.reset(rawMixer, MIX_DestroyMixer);
    subscribe();

    freeTracks_.reserve(tracksCount);
    while (freeTracks_.size() < tracksCount)
    {
        std::shared_ptr<MIX_Track> track(MIX_CreateTrack(mixer_.get()), MIX_DestroyTrack);
        freeTracks_.push_back(std::move(track));
    }
    return true;
}

bool AudioDevice::playSound(const Audio &audio, bool replay)
{
    if (freeTracks_.empty() || !audio)
        return false;

    std::shared_ptr<MIX_Track> track = freeTracks_.back();
    std::shared_ptr<const MIX_Audio> audioS = audio.getSDLAudio().lock();

    AudioPair pair;
    pair.track = track;
    pair.audio = &audio;
    pair.audio->startAudio();

    freeTracks_.pop_back();
    usedTracks_.push_back(std::move(pair));

    MIX_SetTrackAudio(track.get(), const_cast<MIX_Audio *>(audioS.get()));
    MIX_SetTrackGain(track.get(), 1.0f);

    SDL_PropertiesID opts = makePlayOptions(replay ? -1 : 0);
    MIX_PlayTrack(track.get(), opts);
    destroyPlayOptions(opts);

    return true;
}

std::size_t AudioDevice::update()
{
    for (auto it = usedTracks_.begin(); it != usedTracks_.end();)
    {
        // Если не проигрывается или был отключен
        if (!MIX_TrackPlaying(it->track.get()) || !it->audio->isRunnig())
        {
            it->audio->stopAudio();
            MIX_SetTrackAudio(it->track.get(), nullptr); // Отсоединяем звук

            freeTracks_.push_back(it->track); // Возвращаем в пул свободных
            it = usedTracks_.erase(it);
        }
        else
            ++it;
    }
    return freeTracks_.size();
}

std::size_t AudioDevice::getFreeTracksCount() const
{
    return freeTracks_.size();
}
std::size_t AudioDevice::getUsedTracksCount() const
{
    return usedTracks_.size();
}

std::weak_ptr<const MIX_Mixer> AudioDevice::getSDMixer() const
{
    return mixer_;
}
std::weak_ptr<MIX_Mixer> AudioDevice::getSDMixer()
{
    return mixer_;
}

void AudioDevice::setVolumeLevel(const float value)
{
    MIX_SetMixerGain(mixer_.get(), value);
}

} // namespace sdl3::audio