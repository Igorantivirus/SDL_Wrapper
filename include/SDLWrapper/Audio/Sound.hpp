#pragma once

#include <SDLWrapper/Names.hpp>

#include "Audio.hpp"
#include "SoundSettings.hpp"

namespace sdl3::audio
{

class Sound
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

} // namespace sdl3::audio
