// Copyright (c) 2013 Nicolas Silva
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

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
