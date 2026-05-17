#include <SDL_wrapper/audio/SDL3MixerMeneger.hpp>

#include <SDL_wrapper/SDL3GlobalMeneger.hpp>

namespace sdl3::audio
{

bool SDL3MixerMeneger::init(SDL_InitFlags sdlFlags)
{
    if (s_)
        return s_->mixerInited;

    if (!sdl3::SDL3GlobalMeneger::init(sdlFlags))
        return false;

    auto *storage = new Storage();

    if (!MIX_Init())
    {
        delete storage;

        sdl3::SDL3GlobalMeneger::shutdown();

        return false;
    }

    storage->mixerInited = true;

    s_ = storage;

    return true;
}

void SDL3MixerMeneger::shutdown() noexcept
{
    if (!s_)
        return;

    {
        std::lock_guard lk(s_->mtx);

        s_->mixers.clear();
        s_->nextMixerId = 0;
    }

    if (s_->mixerInited)
    {
        MIX_Quit();
        s_->mixerInited = false;
    }

    delete s_;
    s_ = nullptr;

    sdl3::SDL3GlobalMeneger::shutdown();
}

bool SDL3MixerMeneger::isInitialized() noexcept
{
    return s_ != nullptr && s_->mixerInited;
}

SDL3MixerMeneger::Storage *SDL3MixerMeneger::storageOrNull_NoInit() noexcept
{
    return s_;
}

std::size_t SDL3MixerMeneger::subscribeMixer(std::shared_ptr<MIX_Mixer> mixer)
{
    auto *st = storageOrNull_NoInit();

    if (!st)
        return invalidID;

    std::lock_guard lk(st->mtx);

    const auto id = st->nextMixerId++;
    st->mixers[id] = std::move(mixer);

    return id;
}

void SDL3MixerMeneger::unsubscribeMixer(std::size_t id) noexcept
{
    if (id == invalidID)
        return;

    auto *st = storageOrNull_NoInit();

    if (!st)
        return;

    std::lock_guard lk(st->mtx);
    st->mixers.erase(id);
}

std::weak_ptr<MIX_Mixer> SDL3MixerMeneger::getMixer(std::size_t id) noexcept
{
    auto *st = storageOrNull_NoInit();

    if (!st)
        return {};

    std::lock_guard lk(st->mtx);

    auto it = st->mixers.find(id);

    return it == st->mixers.end()
               ? std::weak_ptr<MIX_Mixer>{}
               : it->second;
}

} // namespace sdl3::audio