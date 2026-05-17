#pragma once

#include <SDL_wrapper/Mixer/Export.hpp>

#include <SDL_wrapper/Core/Names.hpp>

#include <SDL_wrapper/Mixer/Audio.hpp>
#include <SDL_wrapper/Mixer/SoundSettings.hpp>

namespace sdl3::mixer
{

class SDL_WRAPPER_MIXER_EXPORT Sound
{
    friend class AudioDevice;

public:
    void setAudio(const Audio &audio);
    const Audio *getAudio() const;

    const LiveControls &getLiveControls() const;
    void updateLiveControls(LiveControls controls);

    bool isPaused() const;
    bool isRunning() const;

    void setPaused(const bool isPaused);
    void stop();

private:
    const Audio *audio_ = nullptr;

    LiveControls controls_;

    bool paused_ = false;
    mutable bool running_ = false;

private:
};

} // namespace sdl3::mixer
