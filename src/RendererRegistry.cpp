#include <SDL_wrapper/Graphics/Detail/RendererRegistry.hpp>

namespace sdl3::detail
{

RendererRegistry::Storage* RendererRegistry::storageOrNull_NoInit() noexcept
{
    return s_;
}

std::size_t RendererRegistry::subscribeRenderer(std::shared_ptr<SDL_Renderer> renderer)
{
    if (!renderer)
        return invalidID;

    if (!s_)
        s_ = new Storage();

    std::lock_guard lk(s_->mtx);

    const auto id = s_->nextRendererId++;
    s_->renderers[id] = std::move(renderer);
    ++s_->usersCount;

    return id;
}

void RendererRegistry::unsubscribeRenderer(std::size_t id) noexcept
{
    if (id == invalidID)
        return;

    auto* st = storageOrNull_NoInit();

    if (!st)
        return;

    bool needDelete = false;

    {
        std::lock_guard lk(st->mtx);

        const auto erased = st->renderers.erase(id);
        if (erased != 0 && st->usersCount > 0)
            --st->usersCount;

        if (st->usersCount == 0)
        {
            st->renderers.clear();
            st->nextRendererId = 0;
            needDelete = true;
        }
    }

    if (needDelete)
    {
        delete st;
        s_ = nullptr;
    }
}

std::weak_ptr<SDL_Renderer> RendererRegistry::getRenderer(std::size_t id) noexcept
{
    auto* st = storageOrNull_NoInit();

    if (!st)
        return {};

    std::lock_guard lk(st->mtx);

    auto it = st->renderers.find(id);

    return it == st->renderers.end()
               ? std::weak_ptr<SDL_Renderer>{}
               : it->second;
}

} // namespace sdl3::detail
