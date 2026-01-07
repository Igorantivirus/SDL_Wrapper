#pragma once

#include <cmath>

#include <SDLWrapper/Names.hpp>

namespace sdl3
{

template <typename T>
float length(const Vector2<T> &v)
{
    return static_cast<float>(std::sqrt(v.x * v.x + v.y * v.y));
}
template <typename T>
float length(const Vector3<T> &v)
{
    return static_cast<float>(std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

template <typename T>
float distance(const Vector2<T> &v1, const Vector2<T> &v2)
{
    return length(v1 - v2);
}
template <typename T>
float distance(const Vector3<T> &v1, const Vector3<T> &v2)
{
    return length(v1 - v2);
}

template <typename T>
Vector2<float> normalize(const Vector2<T> &v)
{
    float len = length(v);
    return len > 0.f ? Vector2<float>{v.x / len, v.y / len} : Vector2<float>{};
}
template <typename T>
Vector3<float> normalize(const Vector3<T> &v)
{
    float len = length(v);
    return len > 0.f ? Vector3<float>{v.x / len, v.y / len, v.z / len} : Vector3<float>{};
}

template <typename T>
T dot(const Vector2<T> &v1, const Vector2<T> &v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}
template <typename T>
T dot(const Vector3<T> &v1, const Vector3<T> &v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template <typename T>
Vector3<T> cross(const Vector3<T> &v1, const Vector3<T> &v2)
{
    return {
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x};
}

} // namespace sdl3
