#pragma once

#include <SDLWrapper/Names.hpp>

namespace sdl3
{

/*
[ a  c  tx ]
[ b  d  ty ]
[ 0  0  1  ]
*/

template <arithmetic T>
struct Matrix3x3
{
    T a, b, c, d, tx, ty;

    Matrix3x3() : a(1), b(0), c(0), d(1), tx(0), ty(0)
    {
    }

    Vector2<T> transform(const Vector2<T> &p) const
    {
        return {
            a * p.x + c * p.y + tx,
            b * p.x + d * p.y + ty};
    }
    Vector3<T> transform(const Vector3<T> &p) const
    {
        return {
            a * p.x + c * p.y + tx * p.z,
            b * p.x + d * p.y + ty * p.z,
            0.0f * p.x + 0.0f * p.y + 1.0f * p.z};
    }

    Matrix3x3 operator*(const Matrix3x3 &other) const
    {
        Matrix3x3 result;
        result.a = a * other.a + c * other.b;
        result.b = b * other.a + d * other.b;
        result.c = a * other.c + c * other.d;
        result.d = b * other.c + d * other.d;
        result.tx = a * other.tx + c * other.ty + tx;
        result.ty = b * other.tx + d * other.ty + ty;
        return result;
    }
};

} // namespace sdl3