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

#include "vodk/gfx/ImageSurface.hpp"

#include <png.h>

namespace vodk {
namespace gfx {

bool ImageSurface::allocate(const IntSize& size, SurfaceFormat f)
{
    printf("ImageSurface::allocate\n");
    uint32_t bpp = bytesPerPixel(f);
    _size = size;
    _stride = bpp * size.width;
    uint32_t buf_size = size.width * size.height * bpp;
    if (buf_size) {
        _data = new byte[buf_size];
    }
}

void ImageSurface::deallocate()
{
    if (_data) {
        delete[] _data;
    }
}

} // namespace
} // namespace
