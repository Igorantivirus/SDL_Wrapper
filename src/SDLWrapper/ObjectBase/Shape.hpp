#pragma once

#include <cmath>
#include <vector>

#include <SDL3/SDL.h>


#include <SDLWrapper/Renders/RenderTarget.hpp>
#include "Drawable.hpp"
#include "SDLWrapper/Texture.hpp"
#include "Transformable.hpp"

namespace sdl3
{

class Shape : public Drawable, public Transformable
{
public:
    virtual ~Shape() = default;

    // ============ Свойства заливки ============
    void setFillColor(const SDL_FColor &color)
    {
        fillColor_ = color;
        updateFillColors();
    }
    const SDL_FColor &getFillColor() const
    {
        return fillColor_;
    }

    // ============ Свойства контура ============
    void setOutlineColor(const SDL_FColor &color)
    {
        outlineColor_ = color;
        updateOutlineColors();
    }
    const SDL_FColor &getOutlineColor() const
    {
        return outlineColor_;
    }

    void setOutlineThickness(float thickness)
    {
        if (outlineThickness_ != thickness)
        {
            outlineThickness_ = thickness;
            update(); // Толщина меняет геометрию outline
        }
    }
    float getOutlineThickness() const
    {
        return outlineThickness_;
    }

    // ============ Работа с текстурой ============
    void setTexture(const Texture &texture)
    {
        texture_ = &texture;
        textureRect_.x = 0;
        textureRect_.y = 0;
        SDL_Point size = texture.getSize();
        textureRect_.w = static_cast<float>(size.x);
        textureRect_.h = static_cast<float>(size.y);
        updateTexCoords();
    }
    void setTexture(const Texture &texture, const SDL_FRect &textureRect)
    {
        texture_ = &texture;
        textureRect_ = textureRect;
        updateTexCoords();
    }
    const Texture *getTexture() const
    {
        return texture_;
    }

    void setTextureRect(const SDL_FRect &textureRect)
    {
        textureRect_ = textureRect;
        updateTexCoords();
    }
    const SDL_FRect &getTextureRect() const
    {
        return textureRect_;
    }

    // ============ Bounds (для UV mapping и collision) ============
    /**
     * @brief Возвращает локальные границы фигуры (без учёта transform).
     * Вычисляется из точек getPoint().
     */
    SDL_FRect getLocalBounds() const
    {
        if (bounds_.w < 0) // Ленивое вычисление (кэш невалиден)
        {
            updateBounds();
        }
        return bounds_;
    }

    /**
     * @brief Возвращает глобальные границы с учётом transform.
     */
    SDL_FRect getGlobalBounds() const
    {
        // TODO: Применить матрицу трансформации из Transformable
        // Пока упрощённая версия (нужна интеграция с твоим Transformable)
        return getLocalBounds();
    }

    // ============ Интерфейс для наследников ============
    /**
     * @brief Количество ключевых точек фигуры (вершин полигона).
     * Для CircleShape это может быть 30, для RectangleShape — 4.
     */
    virtual size_t getPointCount() const = 0;

    /**
     * @brief Координаты точки в локальном пространстве фигуры.
     * @param index Индекс точки [0, getPointCount()).
     * @return Позиция точки относительно origin фигуры.
     */
    virtual SDL_FPoint getPoint(size_t index) const = 0;

    // Метод отрисовки (Double Dispatch)
    void draw(RenderTarget &target) const
    {
        // Double dispatch: делегируем отрисовку RenderTarget,
        // передавая ему нашу геометрию и трансформацию
        target.drawShape(vertices_, outlineVertices_, *this, texture_);
    }

protected:
    Shape() : bounds_{0, 0, -1, -1}
    {
    } // -1 означает "невалидный кэш"

    /**
     * @brief Полностью пересчитывает геометрию (вызывается наследником).
     * Последовательность:
     * 1. Триангулирует fill по getPoint()
     * 2. Строит outline (если thickness > 0)
     * 3. Обновляет цвета и UV
     */
    void update()
    {
        updateFillGeometry();
        buildOutline();
        updateFillColors();
        updateOutlineColors();
        updateTexCoords();
        bounds_.w = -1; // Инвалидируем кэш bounds
    }

    const Texture *texture_ = nullptr;
    SDL_FRect textureRect_ = {0, 0, 0, 0};

    SDL_FColor fillColor_ = {1.0f, 1.0f, 1.0f, 1.0f};
    SDL_FColor outlineColor_ = {1.0f, 1.0f, 1.0f, 1.0f};
    float outlineThickness_ = 0.0f;

    // Кэш геометрии
    std::vector<SDL_Vertex> vertices_;        // Triangle fan/list для заливки
    std::vector<SDL_Vertex> outlineVertices_; // Triangle strip для контура

private:
    mutable SDL_FRect bounds_; // Кэш для getLocalBounds()

    // ============ Внутренние методы обновления ============

    /**
     * @brief Триангулирует фигуру в vertices_.
     * Использует triangle fan с центром фигуры.
     */
    void updateFillGeometry()
    {
        size_t count = getPointCount();
        if (count < 3)
        {
            vertices_.clear();
            return;
        }

        // Triangle fan: центр + N точек = N треугольников
        vertices_.clear();
        vertices_.reserve(3 * count); // N треугольников по 3 вершины

        // Вычисляем центр фигуры
        SDL_FPoint center = {0, 0};
        for (size_t i = 0; i < count; ++i)
        {
            SDL_FPoint p = getPoint(i);
            center.x += p.x;
            center.y += p.y;
        }
        center.x /= count;
        center.y /= count;

        // Генерируем треугольники: центр -> точка[i] -> точка[i+1]
        for (size_t i = 0; i < count; ++i)
        {
            SDL_FPoint p1 = getPoint(i);
            SDL_FPoint p2 = getPoint((i + 1) % count);

            vertices_.push_back({{center.x, center.y}, fillColor_, {0, 0}});
            vertices_.push_back({{p1.x, p1.y}, fillColor_, {0, 0}});
            vertices_.push_back({{p2.x, p2.y}, fillColor_, {0, 0}});
        }
    }

    /**
     * @brief Обновляет цвета заливки во всех вершинах.
     */
    void updateFillColors()
    {
        for (auto &v : vertices_)
        {
            v.color = fillColor_;
        }
    }

    /**
     * @brief Строит outline как quad strip между внешним и внутренним периметрами.
     */
    void buildOutline()
    {
        outlineVertices_.clear();

        if (outlineThickness_ <= 0.0f)
            return;

        size_t count = getPointCount();
        if (count < 3)
            return;

        outlineVertices_.reserve(6 * count); // 2 треугольника на сегмент

        for (size_t i = 0; i < count; ++i)
        {
            size_t next = (i + 1) % count;
            size_t prev = (i == 0) ? count - 1 : i - 1;

            SDL_FPoint p0 = getPoint(prev);
            SDL_FPoint p1 = getPoint(i);
            SDL_FPoint p2 = getPoint(next);

            // Нормаль в точке p1 (усреднённая из двух рёбер)
            SDL_FPoint n1 = computeNormal(p0, p1);
            SDL_FPoint n2 = computeNormal(p1, p2);
            SDL_FPoint normal = {
                (n1.x + n2.x) * 0.5f,
                (n1.y + n2.y) * 0.5f};
            float len = std::sqrt(normal.x * normal.x + normal.y * normal.y);
            if (len > 0.001f)
            {
                normal.x /= len;
                normal.y /= len;
            }

            // Внешняя и внутренняя точки
            SDL_FPoint outer = {
                p1.x + normal.x * outlineThickness_,
                p1.y + normal.y * outlineThickness_};
            SDL_FPoint inner = p1;

            SDL_FPoint p1Next = getPoint(next);
            SDL_FPoint n1Next = computeNormal(p1, p1Next);
            SDL_FPoint n2Next = computeNormal(p1Next, getPoint((next + 1) % count));
            SDL_FPoint normalNext = {
                (n1Next.x + n2Next.x) * 0.5f,
                (n1Next.y + n2Next.y) * 0.5f};
            float lenNext = std::sqrt(normalNext.x * normalNext.x + normalNext.y * normalNext.y);
            if (lenNext > 0.001f)
            {
                normalNext.x /= lenNext;
                normalNext.y /= lenNext;
            }

            SDL_FPoint outerNext = {
                p1Next.x + normalNext.x * outlineThickness_,
                p1Next.y + normalNext.y * outlineThickness_};
            SDL_FPoint innerNext = p1Next;

            // Quad = 2 треугольника
            outlineVertices_.push_back({{inner.x, inner.y}, outlineColor_, {0, 0}});
            outlineVertices_.push_back({{outer.x, outer.y}, outlineColor_, {0, 0}});
            outlineVertices_.push_back({{innerNext.x, innerNext.y}, outlineColor_, {0, 0}});

            outlineVertices_.push_back({{innerNext.x, innerNext.y}, outlineColor_, {0, 0}});
            outlineVertices_.push_back({{outer.x, outer.y}, outlineColor_, {0, 0}});
            outlineVertices_.push_back({{outerNext.x, outerNext.y}, outlineColor_, {0, 0}});
        }
    }

    /**
     * @brief Обновляет цвета контура.
     */
    void updateOutlineColors()
    {
        for (auto &v : outlineVertices_)
        {
            v.color = outlineColor_;
        }
    }

    /**
     * @brief Обновляет UV координаты для текстуры.
     * Маппинг: bounds фигуры -> textureRect_.
     */
    void updateTexCoords()
    {
        if (!texture_)
        {
            // Без текстуры UV не важны
            for (auto &v : vertices_)
            {
                v.tex_coord = {0, 0};
            }
            return;
        }

        SDL_FRect bounds = getLocalBounds();
        SDL_Point texSize = texture_->getSize();

        for (auto &v : vertices_)
        {
            // Нормализованная позиция внутри bounds [0, 1]
            float u = (v.position.x - bounds.x) / bounds.w;
            float v_coord = (v.position.y - bounds.y) / bounds.h;

            // Маппинг на textureRect_
            v.tex_coord.x = (textureRect_.x + u * textureRect_.w) / texSize.x;
            v.tex_coord.y = (textureRect_.y + v_coord * textureRect_.h) / texSize.y;
        }

        // Outline обычно без текстуры, но можно добавить логику
    }

    /**
     * @brief Вычисляет bounds из getPoint().
     */
    void updateBounds() const
    {
        size_t count = getPointCount();
        if (count == 0)
        {
            bounds_ = {0, 0, 0, 0};
            return;
        }

        SDL_FPoint first = getPoint(0);
        float minX = first.x, maxX = first.x;
        float minY = first.y, maxY = first.y;

        for (size_t i = 1; i < count; ++i)
        {
            SDL_FPoint p = getPoint(i);
            minX = std::min(minX, p.x);
            maxX = std::max(maxX, p.x);
            minY = std::min(minY, p.y);
            maxY = std::max(maxY, p.y);
        }

        bounds_ = {minX, minY, maxX - minX, maxY - minY};
    }

    /**
     * @brief Вычисляет перпендикулярную нормаль для ребра p1->p2.
     */
    SDL_FPoint computeNormal(const SDL_FPoint &p1, const SDL_FPoint &p2) const
    {
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        // Перпендикуляр (поворот на 90° против часовой)
        return {-dy, dx};
    }
};

} // namespace sdl3

// #pragma once

// #include <vector>

// #include <SDL3/SDL.h>

// #include "Drawable.hpp"
// #include "SDLWrapper/Texture.hpp"
// #include "Transformable.hpp"

// namespace sdl3
// {

// class Shape : public Drawable, public Transformable
// {
// public:
//     virtual ~Shape() = default;

//     // ============ Свойства заливки ============
//     void setFillColor(const SDL_FColor &color)
//     {
//         fillColor_ = color;
//         updateFillColors();
//     }
//     const SDL_FColor &getFillColor() const
//     {
//         return fillColor_;
//     }

//     // ============ Свойства контура ============
//     void setOutlineColor(const SDL_FColor &color)
//     {
//         outlineColor_ = color;
//         updateOutlineColors();
//     }
//     const SDL_FColor &getOutlineColor() const
//     {
//         return outlineColor_;
//     }

//     void setOutlineThickness(float thickness)
//     {
//         outlineThickness_ = thickness;
//         update(); // Толщина меняет геометрию
//     }
//     float getOutlineThickness() const
//     {
//         return outlineThickness_;
//     }

//     // ============ Работа с текстурой ============
//     void setTexture(const Texture &texture)
//     {
//         texture_ = &texture;
//         textureRect_.x = 0;
//         textureRect_.y = 0;
//         SDL_Point size = texture.getSize();
//         textureRect_.w = size.x;
//         textureRect_.h = size.y;
//     }
//     void setTexture(const Texture &texture, const SDL_FRect &textureRect)
//     {
//         texture_ = &texture;
//         textureRect_ = textureRect;
//     }
//     const Texture *getTexture() const
//     {
//         return texture_;
//     }

//     void setTextureRect(const SDL_FRect &textureRect)
//     {
//         textureRect_ = textureRect;
//     }
//     const SDL_FRect &getTextureRect() const
//     {
//         return textureRect_;
//     }

//     // ============ Интерфейс для наследников ============
//     virtual size_t getPointCount() const = 0;
//     virtual SDL_FPoint getPoint(size_t index) const = 0;

//     // Метод отрисовки (Double Dispatch)
//     void draw(RenderTarget &target) const override;

// protected:
//     Shape() = default;

//     /**
//      * @brief Пересчитывает всю геометрию (вершины, цвета, UV).
//      * Должен вызываться наследником при изменении его параметров (например, радиуса).
//      */
//     void update();

//     const Texture *texture_ = nullptr;
//     SDL_FRect textureRect_ = {0, 0, 0, 0};

//     SDL_FColor fillColor_ = {1.0f, 1.0f, 1.0f, 1.0f}; // Белый по умолчанию
//     SDL_FColor outlineColor_ = {1.0f, 1.0f, 1.0f, 1.0f};
//     float outlineThickness_ = 0.0f;

//     // Кэш геометрии для SDL_RenderGeometry
//     std::vector<SDL_Vertex> vertices_;        // Основное тело фигуры
//     std::vector<SDL_Vertex> outlineVertices_; // Вершины контура

// private:
//     // Внутренние методы обновления частей меша
//     void updateFillColors();
//     void updateOutlineColors();
//     void updateTexCoords();

//     // Вспомогательный метод для построения контура (выдавливание по нормалям)
//     void buildOutline();
// };
// } // namespace sdl3