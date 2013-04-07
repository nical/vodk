
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
