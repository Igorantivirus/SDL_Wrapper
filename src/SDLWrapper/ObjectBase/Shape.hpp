#pragma once

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <vector>

#include <SDL3/SDL.h>

#include "Drawable.hpp"
#include "SDLWrapper/Math/Matrix3x3.hpp"
#include "SDLWrapper/Texture.hpp"
#include "Transformable.hpp"
#include <SDLWrapper/Renders/RenderTarget.hpp>

namespace sdl3
{

class Shape : public Drawable, public Transformable
{
public:
    virtual ~Shape() = default;

    const SDL_FColor &getFillColor() const
    {
        return fillColor_;
    }
    const SDL_FColor &getOutlineColor() const
    {
        return outlineColor_;
    }
    const float getOutlineThickness() const
    {
        return outlineThickness_;
    }
    const Texture *getTexture() const
    {
        return texture_;
    }
    const SDL_FRect &getTextureRect() const
    {
        return textureRect_;
    }
    virtual SDL_FPoint getPoint(const std::size_t index) const = 0;
    virtual std::size_t getPointCount() const = 0;

    void setFillColor(const SDL_FColor &color)
    {
        fillColor_ = color;
        updateFillColors();
    }
    void setOutlineColor(const SDL_FColor &color)
    {
        outlineColor_ = color;
        updateOutlineColors();
    }
    void setOutlineThickness(const float outlineThickness)
    {
        outlineThickness_ = outlineThickness;
        updateOutline();
    }
    void setTexture(const Texture &texture, const SDL_FRect &rect)
    {
        texture_ = &texture;
        textureRect_ = rect;
        updateTexturePoints();
    }
    void setTexture(const Texture &texture)
    {
        texture_ = &texture;
        const SDL_Point &size = texture.getSize();
        textureRect_.x = textureRect_.y = 0;
        textureRect_.w = static_cast<float>(size.x);
        textureRect_.h = static_cast<float>(size.y);
        updateTexturePoints();
    }
    void setTextureRect(const SDL_FRect &rect)
    {
        textureRect_ = rect;
        updateTexturePoints();
    }

protected:
    void updateLocalGeometry()
    {
        updateShape();
        updateOutline();
    }

    void updateShape()
    {
        size_t count = getPointCount();
        if (count < 3)
        {
            localVertices_.clear();
            localOutlineVertices_.clear();
            return;
        }
        localVertices_.clear();
        localVertices_.reserve(count * 3);

        // 1. ВЫЧИСЛЕНИЕ ГРАНИЦ (localBounds_)
        updateLocalBounds();

        // Хелпер для создания вершины с UV-координатами
        auto createVertex = [&](SDL_FPoint p, SDL_FColor color) -> SDL_Vertex
        {
            SDL_Vertex v;
            v.position = p;
            v.color = color;
            v.tex_coord.x = (localBounds_.w != 0) ? (p.x - localBounds_.x) / localBounds_.w : 0.0f;
            v.tex_coord.y = (localBounds_.h != 0) ? (p.y - localBounds_.y) / localBounds_.h : 0.0f;
            return v;
        };

        // 2. ГЕНЕРАЦИЯ ЗАЛИВКИ (localVertices_) - Triangle Fan
        SDL_FPoint center = {localBounds_.x + localBounds_.w / 2.0f, localBounds_.y + localBounds_.h / 2.0f};
        SDL_Vertex centerVert = createVertex(center, fillColor_);

        for (size_t i = 0; i < count; ++i)
        {
            localVertices_.push_back(centerVert);
            localVertices_.push_back(createVertex(getPoint(i), fillColor_));
            localVertices_.push_back(createVertex(getPoint((i + 1) % count), fillColor_));
        }

        // Сбрасываем флаги, помечаем для трансформации
        shapeDirty_ = true;
    }
    void updateOutline()
    {
        size_t count = getPointCount();
        if (count < 3)
        {
            localVertices_.clear();
            localOutlineVertices_.clear();
            return;
        }

        // Хелпер для создания вершины с UV-координатами
        auto createVertex = [&](SDL_FPoint p, SDL_FColor color) -> SDL_Vertex
        {
            SDL_Vertex v;
            v.position = p;
            v.color = color;
            v.tex_coord.x = (localBounds_.w != 0) ? (p.x - localBounds_.x) / localBounds_.w : 0.0f;
            v.tex_coord.y = (localBounds_.h != 0) ? (p.y - localBounds_.y) / localBounds_.h : 0.0f;
            return v;
        };
        // Нормализация векторов ребер
        auto normalize = [](SDL_FPoint v)
        {
            float len = std::sqrt(v.x * v.x + v.y * v.y);
            return (len > 0) ? SDL_FPoint{v.x / len, v.y / len} : SDL_FPoint{0, 0};
        };

        // 3. ГЕНЕРАЦИЯ КОНТУРА (localOutlineVertices_)
        localOutlineVertices_.clear();
        if (outlineThickness_ != 0)
        {
            localOutlineVertices_.reserve(count * 6); // 2 треугольника на каждый сегмент

            for (size_t i = 0; i < count; ++i)
            {
                // Берем текущую, предыдущую и следующую точки для расчета нормалей
                SDL_FPoint pPrev = getPoint((i + count - 1) % count);
                SDL_FPoint pCurr = getPoint(i);
                SDL_FPoint pNext = getPoint((i + 1) % count);

                // Векторы ребер
                SDL_FPoint v1 = {pCurr.x - pPrev.x, pCurr.y - pPrev.y};
                SDL_FPoint v2 = {pNext.x - pCurr.x, pNext.y - pCurr.y};

                v1 = normalize(v1);
                v2 = normalize(v2);

                // Внешние нормали к ребрам (перпендикуляры)
                SDL_FPoint n1 = {-v1.y, v1.x};
                SDL_FPoint n2 = {-v2.y, v2.x};

                // Средняя нормаль в вершине (биссектриса)
                SDL_FPoint edgeNormal = {n1.x + n2.x, n1.y + n2.y};
                edgeNormal = normalize(edgeNormal);

                // Корректировка длины биссектрисы (miter factor), чтобы толщина была везде одинаковой
                float dot = edgeNormal.x * n1.x + edgeNormal.y * n1.y;
                float miterLen = (dot > 0.1f) ? (outlineThickness_ / dot) : outlineThickness_;

                // Точки для "внешнего" и "внутреннего" края контура
                // Если thickness > 0, контур растет наружу, если < 0 - внутрь
                SDL_FPoint outer1 = {pCurr.x + edgeNormal.x * miterLen, pCurr.y + edgeNormal.y * miterLen};
                SDL_FPoint inner1 = pCurr;

                // Аналогично для следующей точки (pNext), чтобы построить Quad
                size_t nextIdx = (i + 1) % count;
                SDL_FPoint pNextNext = getPoint((nextIdx + 1) % count);
                SDL_FPoint v3 = normalize({pNextNext.x - pNext.x, pNextNext.y - pNext.y});
                SDL_FPoint n3 = {-v3.y, v3.x};
                SDL_FPoint edgeNormalNext = normalize({n2.x + n3.x, n2.y + n3.y});
                float dotNext = edgeNormalNext.x * n2.x + edgeNormalNext.y * n2.y;
                float miterLenNext = (dotNext > 0.1f) ? (outlineThickness_ / dotNext) : outlineThickness_;

                SDL_FPoint outer2 = {pNext.x + edgeNormalNext.x * miterLenNext, pNext.y + edgeNormalNext.y * miterLenNext};
                SDL_FPoint inner2 = pNext;

                // Формируем 2 треугольника (Quad) для сегмента контура
                localOutlineVertices_.push_back(createVertex(inner1, outlineColor_));
                localOutlineVertices_.push_back(createVertex(outer1, outlineColor_));
                localOutlineVertices_.push_back(createVertex(outer2, outlineColor_));

                localOutlineVertices_.push_back(createVertex(inner1, outlineColor_));
                localOutlineVertices_.push_back(createVertex(outer2, outlineColor_));
                localOutlineVertices_.push_back(createVertex(inner2, outlineColor_));
            }
        }

        outlineDirty_ = true;
    }

private:
    const Texture *texture_ = nullptr;
    SDL_FRect textureRect_ = {0, 0, 0, 0};

    SDL_FColor fillColor_ = {1.0f, 1.0f, 1.0f, 1.0f};
    SDL_FColor outlineColor_ = {1.0f, 1.0f, 1.0f, 1.0f};
    float outlineThickness_ = 0.0f;

    SDL_FRect localBounds_;
    std::vector<SDL_Vertex> localVertices_;
    std::vector<SDL_Vertex> localOutlineVertices_;

    mutable std::vector<SDL_Vertex> vertices_;
    mutable std::vector<SDL_Vertex> outlineVertices_;
    mutable std::vector<SDL_Vertex> viewVertices_;
    mutable std::vector<SDL_Vertex> viewOutlineVertices_;
    mutable bool shapeDirty_ = true;
    mutable bool outlineDirty_ = true;
    mutable unsigned viewId_ = 0; // С этим работа позже, пока не трогать

private:
    void draw(RenderTarget &target) const override
    {
        if (m_dirty || shapeDirty_)
            updateVertices();
        if (m_dirty || outlineDirty_)
            updateOutlineVertices();
        // Всё передатся по обычной ссылке
        // Там провериться, совпадает ли ID view текущая и у вида окна и при несовпадении массивы обновятся (домножаться на матрицу) и обновится ID.
        const unsigned targetViewId = target.getViewId();
        if (viewId_ != targetViewId)
        {
            viewVertices_ = vertices_;
            viewOutlineVertices_ = outlineVertices_;

            const Matrix3x3 viewMatrix = target.getView().getTransformMatrix();
            const SDL_FPoint screenCenter = target.getTargetCenter();

            for (auto &vert : viewVertices_)
            {
                vert = viewMatrix.transform(vert);
                vert.position.x += screenCenter.x;
                vert.position.y += screenCenter.y;
            }
            for (auto &vert : viewOutlineVertices_)
            {
                vert = viewMatrix.transform(vert);
                vert.position.x += screenCenter.x;
                vert.position.y += screenCenter.y;
            }

            viewId_ = targetViewId;
        }

        target.drawShape(viewVertices_, viewOutlineVertices_, texture_, viewId_);
    }

    void updateLocalBounds()
    {
        SDL_FPoint firstPoint = getPoint(0);
        float minX = firstPoint.x, maxX = firstPoint.x;
        float minY = firstPoint.y, maxY = firstPoint.y;
        std::size_t count = getPointCount();
        for (size_t i = 1; i < count; ++i)
        {
            SDL_FPoint p = getPoint(i);
            if (p.x < minX)
                minX = p.x;
            if (p.x > maxX)
                maxX = p.x;
            if (p.y < minY)
                minY = p.y;
            if (p.y > maxY)
                maxY = p.y;
        }
        localBounds_ = {minX, minY, maxX - minX, maxY - minY};
    }
    void updateFillColors()
    {
        for (auto &v : localVertices_)
            v.color = fillColor_;
        for (auto &v : vertices_)
            v.color = fillColor_;
    }
    void updateOutlineColors()
    {
        for (auto &v : localOutlineVertices_)
            v.color = outlineColor_;
        for (auto &v : outlineVertices_)
            v.color = outlineColor_;
    }
    void updateTexturePoints()
    {
        auto updateTexCoord = [&](SDL_Vertex vertex) -> SDL_FPoint
        {
            SDL_FPoint tex_coord;
            tex_coord.x = (localBounds_.w != 0) ? (vertex.position.x - localBounds_.x) / localBounds_.w : 0.0f;
            tex_coord.y = (localBounds_.h != 0) ? (vertex.position.y - localBounds_.y) / localBounds_.h : 0.0f;
            return tex_coord;
        };
        for (size_t i = 0; i < localVertices_.size(); ++i)
            localVertices_[i].tex_coord = updateTexCoord(localVertices_[i]);
        if (vertices_.size() == localVertices_.size())
        {
            for (size_t i = 0; i < vertices_.size(); ++i)
                vertices_[i].tex_coord = localVertices_[i].tex_coord;
        }
    }

    void updateVertices() const
    {
        shapeDirty_ = false; // Фигура обновлена
        viewId_ = 0;         // Но вид не применён
        Matrix3x3 matrix = getTransformMatrix();

        vertices_.clear();
        vertices_.reserve(localVertices_.size());

        for (const auto &vert : localVertices_)
            vertices_.push_back(matrix.transform(vert));
    }
    void updateOutlineVertices() const
    {
        outlineDirty_ = false; // Фигура обновлена
        viewId_ = 0;           // Но вид не применён
        Matrix3x3 matrix = getTransformMatrix();

        outlineVertices_.clear();
        outlineVertices_.reserve(localOutlineVertices_.size());

        for (const auto &vert : localOutlineVertices_)
            outlineVertices_.push_back(matrix.transform(vert));
    }
};

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
//         if (outlineThickness_ != thickness)
//         {
//             outlineThickness_ = thickness;
//             update(); // Толщина меняет геометрию outline
//         }
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
//         textureRect_.w = static_cast<float>(size.x);
//         textureRect_.h = static_cast<float>(size.y);
//         updateTexCoords();
//     }
//     void setTexture(const Texture &texture, const SDL_FRect &textureRect)
//     {
//         texture_ = &texture;
//         textureRect_ = textureRect;
//         updateTexCoords();
//     }
//     const Texture *getTexture() const
//     {
//         return texture_;
//     }

//     void setTextureRect(const SDL_FRect &textureRect)
//     {
//         textureRect_ = textureRect;
//         updateTexCoords();
//     }
//     const SDL_FRect &getTextureRect() const
//     {
//         return textureRect_;
//     }

//     // ============ Bounds (для UV mapping и collision) ============
//     /**
//      * @brief Возвращает локальные границы фигуры (без учёта transform).
//      * Вычисляется из точек getPoint().
//      */
//     SDL_FRect getLocalBounds() const
//     {
//         if (bounds_.w < 0) // Ленивое вычисление (кэш невалиден)
//         {
//             updateBounds();
//         }
//         return bounds_;
//     }

//     /**
//      * @brief Возвращает глобальные границы с учётом transform.
//      */
//     SDL_FRect getGlobalBounds() const
//     {
//         // TODO: Применить матрицу трансформации из Transformable
//         // Пока упрощённая версия (нужна интеграция с твоим Transformable)
//         return getLocalBounds();
//     }

//     // ============ Интерфейс для наследников ============
//     /**
//      * @brief Количество ключевых точек фигуры (вершин полигона).
//      * Для CircleShape это может быть 30, для RectangleShape — 4.
//      */
//     virtual size_t getPointCount() const = 0;

//     /**
//      * @brief Координаты точки в локальном пространстве фигуры.
//      * @param index Индекс точки [0, getPointCount()).
//      * @return Позиция точки относительно origin фигуры.
//      */
//     virtual SDL_FPoint getPoint(size_t index) const = 0;

//     // Метод отрисовки (Double Dispatch)
//     void draw(RenderTarget &target) const override
//     {
//         // Double dispatch: делегируем отрисовку RenderTarget,
//         // передавая ему нашу геометрию и трансформацию
//         updateTransformed();
//         target.drawShape(transformedVertices_, transformedOutlineVertices_, texture_);
//     }

// protected:
//     Shape() : bounds_{0, 0, -1, -1}
//     {
//     } // -1 означает "невалидный кэш"

//     /**
//      * @brief Полностью пересчитывает геометрию (вызывается наследником).
//      * Последовательность:
//      * 1. Триангулирует fill по getPoint()
//      * 2. Строит outline (если thickness > 0)
//      * 3. Обновляет цвета и UV
//      */
//     void update()
//     {
//         updateFillGeometry();
//         buildOutline();
//         updateFillColors();
//         updateOutlineColors();
//         updateTexCoords();
//         bounds_.w = -1; // Инвалидируем кэш bounds
//         transformDirty_ = true;
//     }

//     const Texture *texture_ = nullptr;
//     SDL_FRect textureRect_ = {0, 0, 0, 0};

//     SDL_FColor fillColor_ = {1.0f, 1.0f, 1.0f, 1.0f};
//     SDL_FColor outlineColor_ = {1.0f, 1.0f, 1.0f, 1.0f};
//     float outlineThickness_ = 0.0f;

//     // Кэш геометрии
//     std::vector<SDL_Vertex> vertices_;        // Triangle fan/list для заливки
//     std::vector<SDL_Vertex> outlineVertices_; // Triangle strip для контура

// private:
//     mutable SDL_FRect bounds_; // Кэш для getLocalBounds()
//     mutable bool transformDirty_ = true;
//     mutable std::vector<SDL_Vertex> transformedVertices_;
//     mutable std::vector<SDL_Vertex> transformedOutlineVertices_;

//     // ============ Внутренние методы обновления ============

//     /**
//      * @brief Триангулирует фигуру в vertices_.
//      * Использует triangle fan с центром фигуры.
//      */
//     void updateFillGeometry()
//     {
//         size_t count = getPointCount();
//         if (count < 3)
//         {
//             vertices_.clear();
//             return;
//         }

//         // Triangle fan: центр + N точек = N треугольников
//         vertices_.clear();
//         vertices_.reserve(3 * count); // N треугольников по 3 вершины

//         // Вычисляем центр фигуры
//         SDL_FPoint center = {0, 0};
//         for (size_t i = 0; i < count; ++i)
//         {
//             SDL_FPoint p = getPoint(i);
//             center.x += p.x;
//             center.y += p.y;
//         }
//         center.x /= count;
//         center.y /= count;

//         // Генерируем треугольники: центр -> точка[i] -> точка[i+1]
//         for (size_t i = 0; i < count; ++i)
//         {
//             SDL_FPoint p1 = getPoint(i);
//             SDL_FPoint p2 = getPoint((i + 1) % count);

//             vertices_.push_back({{center.x, center.y}, fillColor_, {0, 0}});
//             vertices_.push_back({{p1.x, p1.y}, fillColor_, {0, 0}});
//             vertices_.push_back({{p2.x, p2.y}, fillColor_, {0, 0}});
//         }
//     }

//     /**
//      * @brief Обновляет цвета заливки во всех вершинах.
//      */
//     void updateFillColors()
//     {
//         for (auto &v : vertices_)
//         {
//             v.color = fillColor_;
//         }
//         transformDirty_ = true;
//     }

//     /**
//      * @brief Строит outline как quad strip между внешним и внутренним периметрами.
//      */
//     void buildOutline()
//     {
//         outlineVertices_.clear();

//         if (outlineThickness_ <= 0.0f)
//             return;

//         size_t count = getPointCount();
//         if (count < 3)
//             return;

//         outlineVertices_.reserve(6 * count); // 2 треугольника на сегмент

//         for (size_t i = 0; i < count; ++i)
//         {
//             size_t next = (i + 1) % count;
//             size_t prev = (i == 0) ? count - 1 : i - 1;

//             SDL_FPoint p0 = getPoint(prev);
//             SDL_FPoint p1 = getPoint(i);
//             SDL_FPoint p2 = getPoint(next);

//             // Нормаль в точке p1 (усреднённая из двух рёбер)
//             SDL_FPoint n1 = computeNormal(p0, p1);
//             SDL_FPoint n2 = computeNormal(p1, p2);
//             SDL_FPoint normal = {
//                 (n1.x + n2.x) * 0.5f,
//                 (n1.y + n2.y) * 0.5f};
//             float len = std::sqrt(normal.x * normal.x + normal.y * normal.y);
//             if (len > 0.001f)
//             {
//                 normal.x /= len;
//                 normal.y /= len;
//             }

//             // Внешняя и внутренняя точки
//             SDL_FPoint outer = {
//                 p1.x + normal.x * outlineThickness_,
//                 p1.y + normal.y * outlineThickness_};
//             SDL_FPoint inner = p1;

//             SDL_FPoint p1Next = getPoint(next);
//             SDL_FPoint n1Next = computeNormal(p1, p1Next);
//             SDL_FPoint n2Next = computeNormal(p1Next, getPoint((next + 1) % count));
//             SDL_FPoint normalNext = {
//                 (n1Next.x + n2Next.x) * 0.5f,
//                 (n1Next.y + n2Next.y) * 0.5f};
//             float lenNext = std::sqrt(normalNext.x * normalNext.x + normalNext.y * normalNext.y);
//             if (lenNext > 0.001f)
//             {
//                 normalNext.x /= lenNext;
//                 normalNext.y /= lenNext;
//             }

//             SDL_FPoint outerNext = {
//                 p1Next.x + normalNext.x * outlineThickness_,
//                 p1Next.y + normalNext.y * outlineThickness_};
//             SDL_FPoint innerNext = p1Next;

//             // Quad = 2 треугольника
//             outlineVertices_.push_back({{inner.x, inner.y}, outlineColor_, {0, 0}});
//             outlineVertices_.push_back({{outer.x, outer.y}, outlineColor_, {0, 0}});
//             outlineVertices_.push_back({{innerNext.x, innerNext.y}, outlineColor_, {0, 0}});

//             outlineVertices_.push_back({{innerNext.x, innerNext.y}, outlineColor_, {0, 0}});
//             outlineVertices_.push_back({{outer.x, outer.y}, outlineColor_, {0, 0}});
//             outlineVertices_.push_back({{outerNext.x, outerNext.y}, outlineColor_, {0, 0}});
//         }
//     }

//     /**
//      * @brief Обновляет цвета контура.
//      */
//     void updateOutlineColors()
//     {
//         for (auto &v : outlineVertices_)
//         {
//             v.color = outlineColor_;
//         }
//         transformDirty_ = true;
//     }

//     /**
//      * @brief Обновляет UV координаты для текстуры.
//      * Маппинг: bounds фигуры -> textureRect_.
//      */
//     void updateTexCoords()
//     {
//         if (!texture_)
//         {
//             // Без текстуры UV не важны
//             for (auto &v : vertices_)
//             {
//                 v.tex_coord = {0, 0};
//             }
//             transformDirty_ = true;
//             return;
//         }

//         SDL_FRect bounds = getLocalBounds();
//         SDL_Point texSize = texture_->getSize();

//         for (auto &v : vertices_)
//         {
//             // Нормализованная позиция внутри bounds [0, 1]
//             float u = (v.position.x - bounds.x) / bounds.w;
//             float v_coord = (v.position.y - bounds.y) / bounds.h;

//             // Маппинг на textureRect_
//             v.tex_coord.x = (textureRect_.x + u * textureRect_.w) / texSize.x;
//             v.tex_coord.y = (textureRect_.y + v_coord * textureRect_.h) / texSize.y;
//         }
//         transformDirty_ = true;

//         // Outline обычно без текстуры, но можно добавить логику
//     }

//     /**
//      * @brief Вычисляет bounds из getPoint().
//      */
//     void updateBounds() const
//     {
//         size_t count = getPointCount();
//         if (count == 0)
//         {
//             bounds_ = {0, 0, 0, 0};
//             return;
//         }

//         SDL_FPoint first = getPoint(0);
//         float minX = first.x, maxX = first.x;
//         float minY = first.y, maxY = first.y;

//         for (size_t i = 1; i < count; ++i)
//         {
//             SDL_FPoint p = getPoint(i);
//             minX = std::min(minX, p.x);
//             maxX = std::max(maxX, p.x);
//             minY = std::min(minY, p.y);
//             maxY = std::max(maxY, p.y);
//         }

//         bounds_ = {minX, minY, maxX - minX, maxY - minY};
//     }

//     /**
//      * @brief Вычисляет перпендикулярную нормаль для ребра p1->p2.
//      */
//     SDL_FPoint computeNormal(const SDL_FPoint &p1, const SDL_FPoint &p2) const
//     {
//         float dx = p2.x - p1.x;
//         float dy = p2.y - p1.y;
//         // Перпендикуляр (поворот на 90° против часовой)
//         return {-dy, dx};
//     }

// private:
//     void onTransformChanged() override
//     {
//         transformDirty_ = true;
//     }

//     void updateTransformed() const
//     {
//         if (!transformDirty_)
//             return;

//         const SDL_FPoint origin = getOrigin();
//         const SDL_FPoint scale = getScale();
//         const float rotation = getRotation();
//         const SDL_FPoint pos = getPosition();

//         const float rad = -rotation * SDL_PI_F / 180.0f;
//         const float c = std::cos(rad);
//         const float s = std::sin(rad);

//         auto applyTransform = [&](const SDL_Vertex &src, SDL_Vertex &dst)
//         {
//             SDL_FPoint p = src.position;
//             p.x -= origin.x;
//             p.y -= origin.y;

//             p.x *= scale.x;
//             p.y *= scale.y;

//             const float rx = p.x * c - p.y * s;
//             const float ry = p.x * s + p.y * c;

//             dst = src;
//             dst.position.x = rx + pos.x;
//             dst.position.y = ry + pos.y;
//         };

//         transformedVertices_.resize(vertices_.size());
//         for (std::size_t i = 0; i < vertices_.size(); ++i)
//             applyTransform(vertices_[i], transformedVertices_[i]);

//         transformedOutlineVertices_.resize(outlineVertices_.size());
//         for (std::size_t i = 0; i < outlineVertices_.size(); ++i)
//             applyTransform(outlineVertices_[i], transformedOutlineVertices_[i]);

//         transformDirty_ = false;
//     }
//};

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
