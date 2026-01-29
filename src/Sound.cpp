#include <SDLWrapper/Audio/Sound.hpp>

#include <utility>

namespace sdl3::audio
{

void Sound::setAudio(const Audio &audio)
{
    audio_ = &audio;
}

const Audio *Sound::getAudio() const
{
    return audio_;
}

const LiveControls &Sound::getLiveControls() const
{
    return controls_;
}

void Sound::updateLiveControls(LiveControls controls)
{
    controls_ = std::move(controls);
    ++controls_.version_;
}

bool Sound::isPaused() const
{
    return paused_;
}

bool Sound::isRunning() const
{
    return running_;
}

void Sound::setPaused(const bool isPaused)
{
    paused_ = isPaused;
}

void Sound::stop()
{
    running_ = false;
}

} // namespace sdl3::audio
