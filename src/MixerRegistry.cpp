#include <SDL_wrapper/Mixer/Detail/MixerRegistry.hpp>

namespace sdl3::mixer::detail
{

MixerRegistry::Storage* MixerRegistry::storageOrNull_NoInit() noexcept
{
    return s_;
}

std::size_t MixerRegistry::subscribeMixer(std::shared_ptr<MIX_Mixer> mixer)
{
    if (!mixer)
        return invalidID;

    if (!s_)
        s_ = new Storage();

    std::lock_guard lk(s_->mtx);

    const auto id = s_->nextMixerId++;
    s_->mixers[id] = std::move(mixer);
    ++s_->usersCount;

    return id;
}

void MixerRegistry::unsubscribeMixer(std::size_t id) noexcept
{
    if (id == invalidID)
        return;

    auto* st = storageOrNull_NoInit();

    if (!st)
        return;

    bool needDelete = false;

    {
        std::lock_guard lk(st->mtx);

        const auto erased = st->mixers.erase(id);
        if (erased != 0 && st->usersCount > 0)
            --st->usersCount;

        if (st->usersCount == 0)
        {
            st->mixers.clear();
            st->nextMixerId = 0;
            needDelete = true;
        }
    }

    if (needDelete)
    {
        delete st;
        s_ = nullptr;
    }
}

std::weak_ptr<MIX_Mixer> MixerRegistry::getMixer(std::size_t id) noexcept
{
    auto* st = storageOrNull_NoInit();

    if (!st)
        return {};

    std::lock_guard lk(st->mtx);

    auto it = st->mixers.find(id);

    return it == st->mixers.end()
               ? std::weak_ptr<MIX_Mixer>{}
               : it->second;
}

} // namespace sdl3::mixer::detail
