#include <SDLWrapper/ObjectBase/Shape.hpp>

#include <cmath>

#include <SDLWrapper/Renders/RenderTarget.hpp>
#include <SDLWrapper/Texture.hpp>

namespace sdl3
{

const Color &Shape::getFillColor() const
{
    return fillColor_;
}

const Color &Shape::getOutlineColor() const
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

const FloatRect &Shape::getTextureRect() const
{
    return textureRect_;
}

void Shape::setFillColor(const Color &color)
{
    fillColor_ = color;
}

void Shape::setOutlineColor(const Color &color)
{
    outlineColor_ = color;
}

void Shape::setOutlineThickness(const float outlineThickness)
{
    outlineThickness_ = outlineThickness;
    updateLocalOutline();
}

void Shape::setTexture(const Texture &texture, const FloatRect &rect)
{
    texture_ = &texture;
    textureRect_ = rect;
    updateTexturePoints();
}

void Shape::setTexture(const Texture &texture)
{
    texture_ = &texture;
    const Vector2i &size = texture.getSize();
    textureRect_.x = textureRect_.y = 0;
    textureRect_.w = static_cast<float>(size.x);
    textureRect_.h = static_cast<float>(size.y);
    updateTexturePoints();
}

void Shape::setTextureRect(const FloatRect &rect)
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
    const bool viewDirty = viewID_ != target.getViewId();
    const bool transformDirty = m_dirty;

    const bool needFillUpdate = transformDirty || shapeDirty_ || viewDirty;
    const bool needOutlineUpdate = transformDirty || outlineDirty_ || viewDirty;

    if (needFillUpdate || needOutlineUpdate)
    {
        Matrix3x3<float> matrix = target.getView().getTransformMatrix() * getTransformMatrix();
        const Vector2f screenCenter = target.getTargetCenter();
        matrix.tx += screenCenter.x;
        matrix.ty += screenCenter.y;

        if (needFillUpdate)
            updateVertices(matrix);
        if (needOutlineUpdate)
            updateOutlineVertices(matrix);

        viewID_ = target.getViewId();
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
        Vector2i size = texture_->getSize();
        texW = static_cast<float>(size.x);
        texH = static_cast<float>(size.y);
    }

    localVertices_.clear();
    localVertices_.reserve(count * 3);
    textureUV_.clear();
    textureUV_.reserve(count * 3);

    auto createUV = [&](Vector2f p) -> Vector2f
    {
        Vector2f uv;

        float ratioX = (localBounds_.w != 0.0f) ? (p.x - localBounds_.x) / localBounds_.w : 0.0f;
        float ratioY = (localBounds_.h != 0.0f) ? (p.y - localBounds_.y) / localBounds_.h : 0.0f;

        uv.x = (textureRect_.x + ratioX * textureRect_.w) / texW;
        uv.y = (textureRect_.y + ratioY * textureRect_.h) / texH;

        return uv;
    };

    Vector2f center = {localBounds_.x + localBounds_.w / 2.0f, localBounds_.y + localBounds_.h / 2.0f};
    Vector2f centerVert = center;
    Vector2f centerUV = createUV(center);

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

    auto normalizePoint = [](Vector2f v)
    {
        float len = std::sqrt(v.x * v.x + v.y * v.y);
        return (len > 0) ? Vector2f{v.x / len, v.y / len} : Vector2f{0, 0};
    };

    localOutlineVertices_.clear();
    if (outlineThickness_ != 0)
    {
        localOutlineVertices_.reserve(count * 6);

        for (size_t i = 0; i < count; ++i)
        {
            Vector2f pPrev = getPoint((i + count - 1) % count);
            Vector2f pCurr = getPoint(i);
            Vector2f pNext = getPoint((i + 1) % count);

            Vector2f v1 = {pCurr.x - pPrev.x, pCurr.y - pPrev.y};
            Vector2f v2 = {pNext.x - pCurr.x, pNext.y - pCurr.y};

            v1 = normalizePoint(v1);
            v2 = normalizePoint(v2);

            Vector2f n1 = {-v1.y, v1.x};
            Vector2f n2 = {-v2.y, v2.x};

            Vector2f edgeNormal = {n1.x + n2.x, n1.y + n2.y};
            edgeNormal = normalizePoint(edgeNormal);

            float dot = edgeNormal.x * n1.x + edgeNormal.y * n1.y;
            float miterLen = (dot > 0.1f) ? (outlineThickness_ / dot) : outlineThickness_;

            Vector2f outer1 = {pCurr.x + edgeNormal.x * miterLen, pCurr.y + edgeNormal.y * miterLen};
            Vector2f inner1 = pCurr;

            size_t nextIdx = (i + 1) % count;
            Vector2f pNextNext = getPoint((nextIdx + 1) % count);
            Vector2f v3 = normalizePoint({pNextNext.x - pNext.x, pNextNext.y - pNext.y});
            Vector2f n3 = {-v3.y, v3.x};
            Vector2f edgeNormalNext = normalizePoint({n2.x + n3.x, n2.y + n3.y});
            float dotNext = edgeNormalNext.x * n2.x + edgeNormalNext.y * n2.y;
            float miterLenNext = (dotNext > 0.1f) ? (outlineThickness_ / dotNext) : outlineThickness_;

            Vector2f outer2 = {pNext.x + edgeNormalNext.x * miterLenNext, pNext.y + edgeNormalNext.y * miterLenNext};
            Vector2f inner2 = pNext;

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
    Vector2f firstPoint = getPoint(0);
    float minX = firstPoint.x, maxX = firstPoint.x;
    float minY = firstPoint.y, maxY = firstPoint.y;
    std::size_t count = getPointCount();
    for (size_t i = 1; i < count; ++i)
    {
        Vector2f p = getPoint(i);
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
        Vector2i size = texture_->getSize();
        texW = static_cast<float>(size.x);
        texH = static_cast<float>(size.y);
    }

    auto calculateUV = [&](Vector2f pos) -> Vector2f
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

void Shape::updateVertices(const Matrix3x3<float> &matrix) const
{
    shapeDirty_ = false;
    vertices_.clear();
    vertices_.reserve(localVertices_.size());

    for (const auto &vert : localVertices_)
        vertices_.push_back(matrix.transform(vert));
}

void Shape::updateOutlineVertices(const Matrix3x3<float> &matrix) const
{
    outlineDirty_ = false;
    outlineVertices_.clear();
    outlineVertices_.reserve(localOutlineVertices_.size());

    for (const auto &vert : localOutlineVertices_)
        outlineVertices_.push_back(matrix.transform(vert));
}

} // namespace sdl3

