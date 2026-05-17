#pragma once

namespace sdl3
{

template <typename T>
struct Rect
{
    T x{};
    T y{};
    T w{};
    T h{};
};

using IntRect = Rect<int>;
using FloatRect = Rect<float>;

} // namespace sdl3
