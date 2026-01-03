#include <SDLWrapper/Math/Matrix3x3.hpp>

namespace sdl3
{

Matrix3x3::Matrix3x3() : a(1), b(0), c(0), d(1), tx(0), ty(0)
{
}

SDL_FPoint Matrix3x3::transform(const SDL_FPoint p) const
{
    return {
        a * p.x + c * p.y + tx,
        b * p.x + d * p.y + ty};
}

Matrix3x3 Matrix3x3::operator*(const Matrix3x3 &other) const
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

} // namespace sdl3

