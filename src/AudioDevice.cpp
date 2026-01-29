#include "SDL3/SDL_properties.h"
#include "SDL3_mixer/SDL_mixer.h"
#include <SDLWrapper/Audio/AudioDevice.hpp>

#include <SDLWrapper/SDL3GlobalMeneger.hpp>

#include <SDL3/SDL_error.h>

namespace sdl3::audio
{

bool AudioDevice::SoundPair::needDelete()
{
    if (!sound->running_)
        return true;

    auto &live = sound->getLiveControls();
    if (lastSeenVersion != live.version_)
    {
        lastSeenVersion = live.version_;
        live.setControlsToTrack(track);
    }

    if (appliedPaused != sound->paused_)
    {
        const bool ok = sound->paused_ ? MIX_PauseTrack(track.get()) : MIX_ResumeTrack(track.get());
        if (!ok)
            return true;
        appliedPaused = sound->paused_;
    }

    return !MIX_TrackPlaying(track.get()) && !appliedPaused;
}

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

bool AudioDevice::playSound(const Sound &sound, const PlayProperties properties)
{
    if (freeTracks_.empty() || !sound.getAudio())
        return false;

    // Перемещение трека
    std::shared_ptr<MIX_Track> track = freeTracks_.back();

    SoundPair pair;
    pair.track = track;
    pair.sound = &sound;
    pair.lastSeenVersion = sound.getLiveControls().version_;
    pair.appliedPaused = sound.paused_;

    freeTracks_.pop_back();
    usedTracks_.push_back(std::move(pair));

    // Запуск трека
    std::shared_ptr<const MIX_Audio> audioS = sound.getAudio()->getSDLAudio().lock();
    if (!audioS)
        return false;
    SDL_PropertiesID prop = PlayProperties::generateProperties(properties);

    MIX_SetTrackAudio(track.get(), const_cast<MIX_Audio *>(audioS.get()));
    MIX_PlayTrack(track.get(), prop);
    sound.getLiveControls().setControlsToTrack(track);
    if (sound.paused_)
    {
        MIX_PauseTrack(track.get());
    }

    sound.running_ = true;
    if (prop != 0)
        SDL_DestroyProperties(prop);

    return true;
}

std::size_t AudioDevice::update()
{
    for (auto it = usedTracks_.begin(); it != usedTracks_.end();)
    {
        if (it->needDelete())
        {
            it->sound->running_ = false;
            MIX_SetTrackAudio(it->track.get(), nullptr); // Отсоединяем звук
            freeTracks_.push_back(it->track);            // Возвращаем в пул свободных
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

void AudioDevice::setStopedAll(const bool isStoped)
{
    if (isStoped == isStoped_)
        return;
    isStoped_ = isStoped;
    if (isStoped)
        MIX_PauseAllTracks(mixer_.get());
    else
        MIX_ResumeAllTracks(mixer_.get());
}
bool AudioDevice::isStopedAll() const
{
    return isStoped_;
}

} // namespace sdl3::audio
