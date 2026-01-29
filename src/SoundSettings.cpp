#include <SDLWrapper/Audio/SoundSettings.hpp>

#include <cmath> // llround

namespace sdl3::audio
{

PlayProperties PlayProperties::getDefaultProperties()
{
    return PlayProperties();
}

SDL_PropertiesID PlayProperties::generateProperties(const PlayProperties &p)
{
    // Р•СЃР»Рё РІСЃС‘ РїРѕ СѓРјРѕР»С‡Р°РЅРёСЋ вЂ” Р»СѓС‡С€Рµ РІРµСЂРЅСѓС‚СЊ 0: MIX_PlayTrack() РїСЂСЏРјРѕ РґРѕРїСѓСЃРєР°РµС‚ options=0. :contentReference[oaicite:2]{index=2}
    const bool needsProps =
        (p.loopCount != 0) ||
        (p.startMS > 0.f) ||
        (p.loopPointMS > 0.f) ||
        (p.maxMS >= 0.f) ||
        (p.silenceMS > 0.f) ||
        (p.fadeInMS > 0.f) ||
        (p.fadeInStartGain > 0.f);

    if (!needsProps)
        return 0;

    SDL_PropertiesID props = SDL_CreateProperties(); // :contentReference[oaicite:3]{index=3}
    if (props == 0)
        return 0;

    auto setNum = [&](const char *name, float msValue) -> bool
    {
        // Р’ SDL СЌС‚Рѕ Sint64. :contentReference[oaicite:4]{index=4}
        const Sint64 v = (msValue < 0.f) ? (Sint64)-1 : (Sint64)llround(msValue);
        return SDL_SetNumberProperty(props, name, v); // :contentReference[oaicite:5]{index=5}
    };

    // loops
    if (p.loopCount != 0)
    {
        if (!SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, (Sint64)p.loopCount))
        {
            SDL_DestroyProperties(props); // :contentReference[oaicite:6]{index=6}
            return 0;
        }
    }

    // start / loop-start
    if (p.startMS > 0.f)
    {
        if (!setNum(MIX_PROP_PLAY_START_MILLISECOND_NUMBER, p.startMS))
        {
            SDL_DestroyProperties(props);
            return 0;
        }
    }

    if (p.loopPointMS > 0.f)
    {
        if (!setNum(MIX_PROP_PLAY_LOOP_START_MILLISECOND_NUMBER, p.loopPointMS))
        {
            SDL_DestroyProperties(props);
            return 0;
        }
    }

    // max duration/position
    if (p.maxMS >= 0.f)
    {
        if (!setNum(MIX_PROP_PLAY_MAX_MILLISECONDS_NUMBER, p.maxMS))
        {
            SDL_DestroyProperties(props);
            return 0;
        }
    }

    // append silence after loops
    if (p.silenceMS > 0.f)
    {
        if (!setNum(MIX_PROP_PLAY_APPEND_SILENCE_MILLISECONDS_NUMBER, p.silenceMS))
        {
            SDL_DestroyProperties(props);
            return 0;
        }
    }

    // optional fade-in
    if (p.fadeInMS > 0.f)
    {
        if (!setNum(MIX_PROP_PLAY_FADE_IN_MILLISECONDS_NUMBER, p.fadeInMS))
        {
            SDL_DestroyProperties(props);
            return 0;
        }

        // start gain is a FLOAT property, clamped 0..1 by mixer. :contentReference[oaicite:7]{index=7}
        if (p.fadeInStartGain > 0.f)
        {
            if (!SDL_SetFloatProperty(props, MIX_PROP_PLAY_FADE_IN_START_GAIN_FLOAT, p.fadeInStartGain))
            {
                SDL_DestroyProperties(props);
                return 0;
            }
        }
    }

    return props;
}

void LiveControls::setControlsToTrack(std::shared_ptr<MIX_Track> track) const
{
    if (!track)
        return;
    MIX_Track *rtrack = track.get();
    MIX_SetTrackGain(rtrack, volume);
    MIX_SetTrackFrequencyRatio(rtrack, frequencyRatio);
    MIX_Point3D pos;
    pos.x = position.x;
    pos.y = position.y;
    pos.z = position.z;
    MIX_SetTrack3DPosition(rtrack, &pos);
    MIX_StereoGains stereo;
    stereo.left = stereoGains.x;
    stereo.right = stereoGains.y;
    MIX_SetTrackStereo(rtrack, &stereo);
}

} // namespace sdl3::audio

