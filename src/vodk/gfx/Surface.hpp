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

#ifndef VODK_GFX_SURFACE_HPP
#define VODK_GFX_SURFACE_HPP

#include "vodk/gfx/Size.hpp"

#include <stdint.h>

namespace vodk {
namespace gfx {

enum SurfaceFormat {
    SURFACE_R8G8B8A8,
    SURFACE_A8
};

class Surface {
public:
    typedef uint8_t byte;
    virtual SurfaceFormat get_format() const = 0;
    virtual const byte* get_data(int bufIndex = 0) const = 0;
    virtual IntSize get_size() const = 0;
    virtual int get_stride() const = 0;
    virtual int get_skip() const = 0;
    virtual ~Surface() {}
};

uint32_t bytes_per_pixel(SurfaceFormat format);

} // gfx
} // vodk

#endif
