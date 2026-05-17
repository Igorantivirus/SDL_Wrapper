#include <SDL_wrapper/SDL3GlobalMeneger.hpp>

namespace sdl3
{

bool SDL3GlobalMeneger::init(SDL_InitFlags flags)
{
    if (s_)
        return s_->sdlInited;

    auto *storage = new Storage();

    if (!SDL_Init(flags))
    {
        delete storage;
        return false;
    }

    storage->sdlInited = true;
    storage->initFlags = flags;

    s_ = storage;

    return true;
}

void SDL3GlobalMeneger::shutdown() noexcept
{
    if (!s_)
        return;

    {
        std::lock_guard lk(s_->mtx);

        s_->renders.clear();
        s_->nextRenderId = 0;
    }

    if (s_->sdlInited)
    {
        SDL_Quit();
        s_->sdlInited = false;
        s_->initFlags = 0;
    }

    delete s_;
    s_ = nullptr;
}

bool SDL3GlobalMeneger::isInitialized() noexcept
{
    return s_ != nullptr && s_->sdlInited;
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

    return it == st->renders.end()
               ? std::weak_ptr<SDL_Renderer>{}
               : it->second;
}

} // namespace sdl3