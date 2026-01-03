#include <SDLWrapper/ObjectBase/Shape.hpp>

#include <cmath>

#include <SDLWrapper/Renders/RenderTarget.hpp>
#include <SDLWrapper/Texture.hpp>

namespace sdl3
{

const SDL_FColor &Shape::getFillColor() const
{
    return fillColor_;
}

const SDL_FColor &Shape::getOutlineColor() const
{
    return outlineColor_;
}

float Shape::getOutlineThickness() const
{
    return outlineThickness_;
}

const Texture *Shape::getTexture() const
{
    return texture_;
}

const SDL_FRect &Shape::getTextureRect() const
{
    return textureRect_;
}

void Shape::setFillColor(const SDL_FColor &color)
{
    fillColor_ = color;
}

void Shape::setOutlineColor(const SDL_FColor &color)
{
    outlineColor_ = color;
}

void Shape::setOutlineThickness(const float outlineThickness)
{
    outlineThickness_ = outlineThickness;
    updateLocalOutline();
}

void Shape::setTexture(const Texture &texture, const SDL_FRect &rect)
{
    texture_ = &texture;
    textureRect_ = rect;
    updateTexturePoints();
}

void Shape::setTexture(const Texture &texture)
{
    texture_ = &texture;
    const SDL_Point &size = texture.getSize();
    textureRect_.x = textureRect_.y = 0;
    textureRect_.w = static_cast<float>(size.x);
    textureRect_.h = static_cast<float>(size.y);
    updateTexturePoints();
}

void Shape::setTextureRect(const SDL_FRect &rect)
{
    textureRect_ = rect;
    updateTexturePoints();
}

void Shape::updateLocalGeometry()
{
    updateLocalShape();
    updateLocalOutline();
}

void Shape::draw(RenderTarget &target) const
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

    target.drawShape(texture_,
                     vertices_.data(), static_cast<int>(vertices_.size()),
                     textureUV_.data(), static_cast<int>(textureUV_.size()),
                     fillColor_,
                     nullptr, 0);

    target.drawShape(nullptr,
                     outlineVertices_.data(), static_cast<int>(outlineVertices_.size()),
                     nullptr, 0,
                     outlineColor_,
                     nullptr, 0);
}

void Shape::updateLocalShape()
{
    size_t count = getPointCount();
    if (count < 3)
    {
        localVertices_.clear();
        localOutlineVertices_.clear();
        return;
    }

    updateLocalBounds();

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

    auto createUV = [&](SDL_FPoint p) -> SDL_FPoint
    {
        SDL_FPoint uv;

        float ratioX = (localBounds_.w != 0.0f) ? (p.x - localBounds_.x) / localBounds_.w : 0.0f;
        float ratioY = (localBounds_.h != 0.0f) ? (p.y - localBounds_.y) / localBounds_.h : 0.0f;

        uv.x = (textureRect_.x + ratioX * textureRect_.w) / texW;
        uv.y = (textureRect_.y + ratioY * textureRect_.h) / texH;

        return uv;
    };

    SDL_FPoint center = {localBounds_.x + localBounds_.w / 2.0f, localBounds_.y + localBounds_.h / 2.0f};
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

void Shape::updateLocalOutline()
{
    size_t count = getPointCount();
    if (count < 3)
    {
        localVertices_.clear();
        localOutlineVertices_.clear();
        return;
    }

    auto normalizePoint = [](SDL_FPoint v)
    {
        float len = std::sqrt(v.x * v.x + v.y * v.y);
        return (len > 0) ? SDL_FPoint{v.x / len, v.y / len} : SDL_FPoint{0, 0};
    };

    localOutlineVertices_.clear();
    if (outlineThickness_ != 0)
    {
        localOutlineVertices_.reserve(count * 6);

        for (size_t i = 0; i < count; ++i)
        {
            SDL_FPoint pPrev = getPoint((i + count - 1) % count);
            SDL_FPoint pCurr = getPoint(i);
            SDL_FPoint pNext = getPoint((i + 1) % count);

            SDL_FPoint v1 = {pCurr.x - pPrev.x, pCurr.y - pPrev.y};
            SDL_FPoint v2 = {pNext.x - pCurr.x, pNext.y - pCurr.y};

            v1 = normalizePoint(v1);
            v2 = normalizePoint(v2);

            SDL_FPoint n1 = {-v1.y, v1.x};
            SDL_FPoint n2 = {-v2.y, v2.x};

            SDL_FPoint edgeNormal = {n1.x + n2.x, n1.y + n2.y};
            edgeNormal = normalizePoint(edgeNormal);

            float dot = edgeNormal.x * n1.x + edgeNormal.y * n1.y;
            float miterLen = (dot > 0.1f) ? (outlineThickness_ / dot) : outlineThickness_;

            SDL_FPoint outer1 = {pCurr.x + edgeNormal.x * miterLen, pCurr.y + edgeNormal.y * miterLen};
            SDL_FPoint inner1 = pCurr;

            size_t nextIdx = (i + 1) % count;
            SDL_FPoint pNextNext = getPoint((nextIdx + 1) % count);
            SDL_FPoint v3 = normalizePoint({pNextNext.x - pNext.x, pNextNext.y - pNext.y});
            SDL_FPoint n3 = {-v3.y, v3.x};
            SDL_FPoint edgeNormalNext = normalizePoint({n2.x + n3.x, n2.y + n3.y});
            float dotNext = edgeNormalNext.x * n2.x + edgeNormalNext.y * n2.y;
            float miterLenNext = (dotNext > 0.1f) ? (outlineThickness_ / dotNext) : outlineThickness_;

            SDL_FPoint outer2 = {pNext.x + edgeNormalNext.x * miterLenNext, pNext.y + edgeNormalNext.y * miterLenNext};
            SDL_FPoint inner2 = pNext;

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

void Shape::updateLocalBounds()
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

void Shape::updateTexturePoints()
{
    if (localVertices_.empty())
        return;

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
        float ratioX = (localBounds_.w != 0) ? (pos.x - localBounds_.x) / localBounds_.w : 0.0f;
        float ratioY = (localBounds_.h != 0) ? (pos.y - localBounds_.y) / localBounds_.h : 0.0f;

        float px = textureRect_.x + ratioX * textureRect_.w;
        float py = textureRect_.y + ratioY * textureRect_.h;

        return {px / texW, py / texH};
    };

    textureUV_.clear();
    textureUV_.reserve(localVertices_.size());
    for (auto &p : localVertices_)
        textureUV_.push_back(calculateUV(p));
}

void Shape::updateVertices(const Matrix3x3 &matrix) const
{
    shapeDirty_ = false;
    viewId_ = 0;
    vertices_.clear();
    vertices_.reserve(localVertices_.size());

    for (const auto &vert : localVertices_)
        vertices_.push_back(matrix.transform(vert));
}

void Shape::updateOutlineVertices(const Matrix3x3 &matrix) const
{
    outlineDirty_ = false;
    viewId_ = 0;
    outlineVertices_.clear();
    outlineVertices_.reserve(localOutlineVertices_.size());

    for (const auto &vert : localOutlineVertices_)
        outlineVertices_.push_back(matrix.transform(vert));
}

} // namespace sdl3

