#include <SDLWrapper/SDL3GlobalMeneger.hpp>

namespace sdl3
{

/// Инициализация SDL и/или SDL_mixer
bool SDL3GlobalMeneger::init(bool initSDL, bool initMixer)
{
    if (!s_)
        s_ = new Storage();

    s_->wantSDL = s_->wantSDL || initSDL;
    s_->wantMixer = s_->wantMixer || initMixer;

    bool ok = true;

    // SDL
    if (initSDL && !s_->sdlInited)
    {
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
        { // bool: true = ok, false = fail
            s_->sdlInited = true;
        }
        else
        {
            ok = false;
        }
    }

    // SDL_mixer
    if (initMixer && !s_->mixerInited)
    {
        if (MIX_Init())
        {
            s_->mixerInited = true;
        }
        else
        {
            ok = false;
        }
    }

    return ok;
}

/// Завершение: вызывает Quit только для тех, что реально были инициализированы.
void SDL3GlobalMeneger::shutdown() noexcept
{
    if (!s_)
        return;

    if (s_->mixerInited)
    {
        MIX_Quit();
        s_->mixerInited = false;
    }

    if (s_->sdlInited)
    {
        SDL_Quit();
        s_->sdlInited = false;
    }

    delete s_;
    s_ = nullptr;
}

bool SDL3GlobalMeneger::isInitialized() noexcept
{
    return s_ != nullptr;
}

SDL3GlobalMeneger::Storage *SDL3GlobalMeneger::storageOrNull_NoInit() noexcept
{
    return s_;
}

std::size_t SDL3GlobalMeneger::subscribeRenderer(std::shared_ptr<SDL_Renderer> render)
{
    auto *st = storageOrNull_NoInit();
    if (!st)
        return invalidID;
    std::lock_guard lk(st->mtx);
    const auto id = st->nextRenderId++;
    st->renders[id] = std::move(render);
    return id;
}

void SDL3GlobalMeneger::unsubscribeRenderer(std::size_t id) noexcept
{
    if (id == invalidID)
        return;
    auto *st = storageOrNull_NoInit();
    if (!st)
        return;
    std::lock_guard lk(st->mtx);
    st->renders.erase(id);
}

std::weak_ptr<SDL_Renderer> SDL3GlobalMeneger::getRenderer(std::size_t id) noexcept
{
    auto *st = storageOrNull_NoInit();
    if (!st)
        return {};
    std::lock_guard lk(st->mtx);
    auto it = st->renders.find(id);
    return it == st->renders.end() ? std::weak_ptr<SDL_Renderer>{} : it->second;
}

std::size_t SDL3GlobalMeneger::subscribeMixer(std::shared_ptr<MIX_Mixer> mixer)
{
    auto *st = storageOrNull_NoInit();
    if (!st)
        return invalidID;
    std::lock_guard lk(st->mtx);
    const auto id = st->nextMixerId++;
    st->mixers[id] = std::move(mixer);
    return id;
}

void SDL3GlobalMeneger::unsubscribeMixer(std::size_t id) noexcept
{
    if (id == invalidID)
        return;
    auto *st = storageOrNull_NoInit();
    if (!st)
        return;
    std::lock_guard lk(st->mtx);
    st->mixers.erase(id);
}

std::weak_ptr<MIX_Mixer> SDL3GlobalMeneger::getMixer(std::size_t id) noexcept
{
    auto *st = storageOrNull_NoInit();
    if (!st)
        return {};
    std::lock_guard lk(st->mtx);
    auto it = st->mixers.find(id);
    return it == st->mixers.end() ? std::weak_ptr<MIX_Mixer>{} : it->second;
}
} // namespace sdl3