#ifndef VODK_GFX_RECT_HPP
#define VODK_GFX_RECT_HPP

#include "vodk/core/Range.hpp"
#include "vodk/gfx/Size.hpp"

#include "stdint.h"

namespace vodk {
namespace gfx {

template<typename T>
struct Rect
{
    T x;
    T y;
    T width;
    T height;

    Rect(T aX, T aY, T aW, T aH)
    : x(aX), y(aY), width(aW), height(aH)
    {}

    Size<T> size() const { return Size<T>(width, height); }

    Range<T> range() { return Range<T>(&x, 4); }

    Range<const T> range() const { return Range<const T>(&x, 4); }

    T xMost() const { return x + width; }

    T yMost() const { return y + height; }
};

typedef Rect<float> FloatRect;
typedef Rect<uint32_t> IntRect;

} // namespace
} // namespace

#endif
