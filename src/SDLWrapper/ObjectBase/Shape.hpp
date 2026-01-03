#pragma once

#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

#include <SDLWrapper/Renders/RenderTarget.hpp>

#include "Drawable.hpp"
#include "SDLWrapper/Math/Matrix3x3.hpp"
#include "SDLWrapper/Texture.hpp"
#include "Transformable.hpp"

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
    }
    void setOutlineColor(const SDL_FColor &color)
    {
        outlineColor_ = color;
    }
    void setOutlineThickness(const float outlineThickness)
    {
        outlineThickness_ = outlineThickness;
        updateLocalOutline();
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
        updateLocalShape();
        updateLocalOutline();
    }

private:
    const Texture *texture_ = nullptr;
    SDL_FRect textureRect_ = {0, 0, 0, 0};

    SDL_FColor fillColor_ = {1.0f, 1.0f, 1.0f, 1.0f};
    SDL_FColor outlineColor_ = {1.0f, 1.0f, 1.0f, 1.0f};
    float outlineThickness_ = 0.0f;

    SDL_FRect localBounds_;
    std::vector<SDL_FPoint> textureUV_;
    std::vector<SDL_FPoint> localVertices_;
    std::vector<SDL_FPoint> localOutlineVertices_;

    mutable std::vector<SDL_FPoint> vertices_;
    mutable std::vector<SDL_FPoint> outlineVertices_;
    mutable bool shapeDirty_ = true;
    mutable bool outlineDirty_ = true;
    mutable unsigned viewId_ = 0;

private:
    void draw(RenderTarget &target) const override
    {
        const bool viewDirty = viewId_ != target.getViewId();
        const bool transformDirty = m_dirty;

        const bool needFillUpdate = transformDirty || shapeDirty_ || viewDirty;
        const bool needOutlineUpdate = transformDirty || outlineDirty_ || viewDirty;

        if (needFillUpdate || needOutlineUpdate)
        {
            Matrix3x3 matrix = target.getView().getTransformMatrix() * getTransformMatrix();
            const SDL_FPoint screenCenter = target.getTargetCenter();
            matrix.tx += screenCenter.x;
            matrix.ty += screenCenter.y;

            if (needFillUpdate)
                updateVertices(matrix);
            if (needOutlineUpdate)
                updateOutlineVertices(matrix);

            viewId_ = target.getViewId();
        }
        target.drawShape(texture_, vertices_, fillColor_, textureUV_);
        target.drawShape(outlineVertices_, outlineColor_);
    }

    void updateLocalShape()
    {
        size_t count = getPointCount();
        if (count < 3)
        {
            localVertices_.clear();
            localOutlineVertices_.clear();
            return;
        }

        // 1. ВЫЧИСЛЕНИЕ ГРАНИЦ (localBounds_) - 1-й проход O(N)
        updateLocalBounds();

        // Предварительно вычисляем данные текстуры, чтобы не делать этого в цикле
        float texW = 1.0f;
        float texH = 1.0f;
        if (texture_)
        {
            SDL_Point size = texture_->getSize();
            texW = static_cast<float>(size.x);
            texH = static_cast<float>(size.y);
        }

        localVertices_.clear();
        localVertices_.reserve(count * 3);
        textureUV_.clear();
        textureUV_.reserve(count * 3);

        // Хелпер для создания вершины: Геометрия + UV + Цвет в один присест - 2-й проход O(N)
        auto createUV = [&](SDL_FPoint p) -> SDL_FPoint
        {
            SDL_FPoint uv;

            // Коэффициент положения точки в фигуре (0.0 - 1.0)
            float ratioX = (localBounds_.w != 0.0f) ? (p.x - localBounds_.x) / localBounds_.w : 0.0f;
            float ratioY = (localBounds_.h != 0.0f) ? (p.y - localBounds_.y) / localBounds_.h : 0.0f;

            // Накладываем textureRect и нормализуем для GPU
            uv.x = (textureRect_.x + ratioX * textureRect_.w) / texW;
            uv.y = (textureRect_.y + ratioY * textureRect_.h) / texH;

            return uv;
        };

        // 2. ГЕНЕРАЦИЯ ЗАЛИВКИ (Triangle Fan)
        SDL_FPoint center = {localBounds_.x + localBounds_.w / 2.0f, localBounds_.y + localBounds_.h / 2.0f};
        // ShapeVertex centerVert = createFullVertex(center);
        SDL_FPoint centerVert = center;
        SDL_FPoint centerUV = createUV(center);

        for (size_t i = 0; i < count; ++i)
        {
            localVertices_.push_back(centerVert);
            localVertices_.push_back(getPoint(i));
            localVertices_.push_back(getPoint((i + 1) % count));

            textureUV_.push_back(centerUV);
            textureUV_.push_back(createUV(getPoint(i)));
            textureUV_.push_back(createUV(getPoint((i + 1) % count)));
        }
        shapeDirty_ = true;
    }

    void updateLocalOutline()
    {
        size_t count = getPointCount();
        if (count < 3)
        {
            localVertices_.clear();
            localOutlineVertices_.clear();
            return;
        }

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
                localOutlineVertices_.push_back(inner1);
                localOutlineVertices_.push_back(outer1);
                localOutlineVertices_.push_back(outer2);

                localOutlineVertices_.push_back(inner1);
                localOutlineVertices_.push_back(outer2);
                localOutlineVertices_.push_back(inner2);
            }
        }

        outlineDirty_ = true;
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
    void updateTexturePoints()
    {
        if (localVertices_.empty())
            return;

        // Получаем размер текстуры для нормализации
        float texW = 1.0f;
        float texH = 1.0f;
        if (texture_)
        {
            SDL_Point size = texture_->getSize();
            texW = static_cast<float>(size.x);
            texH = static_cast<float>(size.y);
        }

        auto calculateUV = [&](SDL_FPoint pos) -> SDL_FPoint
        {
            // 1. Находим относительную позицию точки в локальных границах фигуры (0.0 - 1.0)
            float ratioX = (localBounds_.w != 0) ? (pos.x - localBounds_.x) / localBounds_.w : 0.0f;
            float ratioY = (localBounds_.h != 0) ? (pos.y - localBounds_.y) / localBounds_.h : 0.0f;

            // 2. Проецируем этот коэффициент на textureRect (в пикселях)
            float px = textureRect_.x + ratioX * textureRect_.w;
            float py = textureRect_.y + ratioY * textureRect_.h;

            // 3. Нормализуем координаты для GPU (делим пиксели на общий размер текстуры)
            return {px / texW, py / texH};
        };

        // Обновляем локальный "эталон"
        textureUV_.clear();
        textureUV_.reserve(localVertices_.size());
        for (auto &p : localVertices_)
            textureUV_.push_back(calculateUV(p));
    }

    void updateVertices(const Matrix3x3 &matrix) const
    {
        shapeDirty_ = false; // Фигура обновлена
        viewId_ = 0;         // Но вид не применён
        vertices_.clear();
        vertices_.reserve(localVertices_.size());

        for (const auto &vert : localVertices_)
            vertices_.push_back(matrix.transform(vert));
    }
    void updateOutlineVertices(const Matrix3x3 &matrix) const
    {
        outlineDirty_ = false; // Фигура обновлена
        viewId_ = 0;           // Но вид не применён
        outlineVertices_.clear();
        outlineVertices_.reserve(localOutlineVertices_.size());

        for (const auto &vert : localOutlineVertices_)
            outlineVertices_.push_back(matrix.transform(vert));
    }
};

} // namespace sdl3