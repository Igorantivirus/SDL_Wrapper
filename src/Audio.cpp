#include <SDLWrapper/Audio/Audio.hpp>
#include <SDLWrapper/SDL3GlobalMeneger.hpp>

#include <SDL3/SDL_error.h>
#include <SDL3_mixer/SDL_mixer.h>

namespace sdl3::audio
{

Audio::Audio(std::size_t deviceID) : deviceID_(deviceID)
{
}

void Audio::stop()
{
    isRunning_ = false;
}

bool Audio::loadFromFile(const char *path, bool predecode)
{
    std::shared_ptr<MIX_Mixer> mixerS = SDL3GlobalMeneger::getMixer(deviceID_).lock();
    if (!mixerS)
    {
        SDL_Log("There is no mixer to load the audio");
        return false;
    }

    MIX_Audio *audio = MIX_LoadAudio(mixerS.get(), path, predecode);
    if (!audio)
    {
        SDL_Log("%s\n", SDL_GetError());
        return false;
    }
    audio_.reset(audio, MIX_DestroyAudio);
    isRunning_ = false;
    return true;
}

std::weak_ptr<const MIX_Audio> Audio::getSDLAudio() const
{
    return audio_;
}
std::weak_ptr<MIX_Audio> Audio::getSDLAudio()
{
    return audio_;
}

Audio::operator bool() const
{
    return static_cast<bool>(audio_);
}

bool Audio::isRunnig() const
{
    return isRunning_;
}

} // namespace sdl3::audio
