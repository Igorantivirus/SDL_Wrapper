#pragma once

#include <cmath>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_stdinc.h>

namespace sdl3
{

// Матрица 3x3 для 2D трансформаций (affine transformations)
// Используем column-major order для удобства
struct Matrix3x3
{
    // m[column][row]
    float m[3][3];

    Matrix3x3()
    {
        identity();
    }

    void identity()
    {
        m[0][0] = 1.f; m[1][0] = 0.f; m[2][0] = 0.f;
        m[0][1] = 0.f; m[1][1] = 1.f; m[2][1] = 0.f;
        m[0][2] = 0.f; m[1][2] = 0.f; m[2][2] = 1.f;
    }

    // Умножение матриц: result = this * other
    Matrix3x3 operator*(const Matrix3x3 &other) const
    {
        Matrix3x3 result;
        
        for (int col = 0; col < 3; ++col)
        {
            for (int row = 0; row < 3; ++row)
            {
                result.m[col][row] = 
                    m[0][row] * other.m[col][0] +
                    m[1][row] * other.m[col][1] +
                    m[2][row] * other.m[col][2];
            }
        }
        
        return result;
    }

    // Применение матрицы к точке (x, y, 1)
    SDL_FPoint transformPoint(const SDL_FPoint &point) const
    {
        return SDL_FPoint{
            m[0][0] * point.x + m[1][0] * point.y + m[2][0],
            m[0][1] * point.x + m[1][1] * point.y + m[2][1]
        };
    }

    // Построение матрицы трансформации объекта
    // Порядок: Scale -> Rotate -> Translate
    static Matrix3x3 createTransform(const SDL_FPoint &position, 
                                      const SDL_FPoint &origin,
                                      const SDL_FPoint &scale, 
                                      float rotationDegrees)
    {
        Matrix3x3 result;
        
        const float rad = rotationDegrees * SDL_PI_F / 180.0f;
        const float cosA = std::cos(rad);
        const float sinA = std::sin(rad);

        // Комбинированная матрица: Translate * Rotate * Scale * (-Origin)
        // Сначала сдвигаем на -origin, потом scale, rotate, translate
        
        const float sxc = scale.x * cosA;
        const float syc = scale.y * cosA;
        const float sxs = scale.x * sinA;
        const float sys = scale.y * sinA;

        result.m[0][0] = sxc;
        result.m[1][0] = -sys;
        result.m[2][0] = position.x - (origin.x * sxc - origin.y * sys);

        result.m[0][1] = sxs;
        result.m[1][1] = syc;
        result.m[2][1] = position.y - (origin.x * sxs + origin.y * syc);

        result.m[0][2] = 0.f;
        result.m[1][2] = 0.f;
        result.m[2][2] = 1.f;

        return result;
    }

    // Построение матрицы камеры (view matrix)
    // Инверсная трансформация: центр камеры становится (0,0), учитываем zoom и rotation
    static Matrix3x3 createView(const SDL_FPoint &center,
                                 const SDL_FPoint &zoom,
                                 float angleDegrees,
                                 const SDL_FPoint &screenSize)
    {
        Matrix3x3 result;

        const float rad = -angleDegrees * SDL_PI_F / 180.0f; // Инверсия угла
        const float cosA = std::cos(rad);
        const float sinA = std::sin(rad);

        // View matrix = Translate(screenCenter) * Scale(zoom) * Rotate(-angle) * Translate(-center)
        
        const float halfW = screenSize.x * 0.5f;
        const float halfH = screenSize.y * 0.5f;

        const float zxc = zoom.x * cosA;
        const float zyc = zoom.y * cosA;
        const float zxs = zoom.x * sinA;
        const float zys = zoom.y * sinA;

        result.m[0][0] = zxc;
        result.m[1][0] = zys;
        result.m[2][0] = halfW - (center.x * zxc + center.y * zys);

        result.m[0][1] = -zxs;
        result.m[1][1] = zyc;
        result.m[2][1] = halfH - (-center.x * zxs + center.y * zyc);

        result.m[0][2] = 0.f;
        result.m[1][2] = 0.f;
        result.m[2][2] = 1.f;

        return result;
    }
};

} // namespace sdl3
