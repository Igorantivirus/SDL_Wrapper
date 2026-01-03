#include <SDLWrapper/Renders/RenderMeneger.hpp>

namespace sdl3
{

std::unordered_map<std::size_t, std::weak_ptr<SDL_Renderer>> RenderMeneger::renders_{};

std::size_t RenderMeneger::subscribeRenderer(std::shared_ptr<SDL_Renderer> render)
{
    static std::size_t nextId = 0;
    renders_[nextId] = std::move(render);
    return nextId++;
}

void RenderMeneger::unsubscribeRenderer(const std::size_t id)
{
    renders_.erase(id);
}

std::weak_ptr<SDL_Renderer> RenderMeneger::getRenderer(const std::size_t id)
{
    auto found = renders_.find(id);
    return found == renders_.end() ? std::weak_ptr<SDL_Renderer>{} : found->second;
}

} // namespace sdl3
