#pragma once

#include <concepts>

namespace sdl3
{

template <typename T>
concept arithmetic = std::integral<T> || std::floating_point<T>;

template <arithmetic T>
struct Vector2
{
    T x{};
    T y{};

    // Конструкторы
    Vector2() = default;
    Vector2(T x, T y) : x(x), y(y)
    {
    }
    explicit Vector2(T scalar) : x(scalar), y(scalar)
    {
    }

    // Унарные операторы
    Vector2 operator+() const
    {
        return *this;
    }
    Vector2 operator-() const
    {
        return {-x, -y};
    }

    // Арифметические операторы с векторами
    Vector2 operator+(const Vector2 &other) const
    {
        return {x + other.x, y + other.y};
    }
    Vector2 operator-(const Vector2 &other) const
    {
        return {x - other.x, y - other.y};
    }

    // Операторы присваивания с векторами
    Vector2 &operator+=(const Vector2 &other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }
    Vector2 &operator-=(const Vector2 &other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    // Арифметические операторы со скалярами (обе стороны)
    Vector2 operator*(T scalar) const
    {
        return {x * scalar, y * scalar};
    }
    Vector2 operator/(T scalar) const
    {
        return {x / scalar, y / scalar};
    }

    // Операторы присваивания со скалярами
    Vector2 &operator*=(T scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    Vector2 &operator/=(T scalar)
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    // Сравнение
    bool operator==(const Vector2 &other) const
    {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Vector2 &other) const
    {
        return !this->operator==(other);
    }
};

// Дружественные операторы для скаляра слева
template <arithmetic T>
Vector2<T> operator*(T scalar, const Vector2<T> &vec)
{
    return {scalar * vec.x, scalar * vec.y};
}

// ==================== Vector3 ====================

template <arithmetic T>
struct Vector3
{
    T x{};
    T y{};
    T z{};

    // Конструкторы
    Vector3() = default;
    Vector3(T x, T y, T z) : x(x), y(y), z(z)
    {
    }
    explicit Vector3(T scalar) : x(scalar), y(scalar), z(scalar)
    {
    }

    // Унарные операторы
    Vector3 operator+() const
    {
        return *this;
    }
    Vector3 operator-() const
    {
        return {-x, -y, -z};
    }

    // Арифметические операторы с векторами
    Vector3 operator+(const Vector3 &other) const
    {
        return {x + other.x, y + other.y, z + other.z};
    }
    Vector3 operator-(const Vector3 &other) const
    {
        return {x - other.x, y - other.y, z - other.z};
    }

    // Операторы присваивания с векторами
    Vector3 &operator+=(const Vector3 &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    Vector3 &operator-=(const Vector3 &other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    // Арифметические операторы со скалярами (обе стороны)
    Vector3 operator*(T scalar) const
    {
        return {x * scalar, y * scalar, z * scalar};
    }
    Vector3 operator/(T scalar) const
    {
        return {x / scalar, y / scalar, z / scalar};
    }

    // Операторы присваивания со скалярами
    Vector3 &operator*=(T scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    Vector3 &operator/=(T scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    // Сравнение
    bool operator==(const Vector3 &other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }
    bool operator!=(const Vector3 &other) const
    {
        return !this->operator==(other);
    }
};

template <arithmetic T>
Vector3<T> operator*(T scalar, const Vector3<T> &vec)
{
    return {scalar * vec.x, scalar * vec.y, scalar * vec.z};
}

// Псевдонимы для распространенных типов
using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;
using Vector2i = Vector2<int>;
using Vector2u = Vector2<unsigned>;

using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;
using Vector3i = Vector3<int>;
using Vector3u = Vector3<unsigned>;

} // namespace sdl3