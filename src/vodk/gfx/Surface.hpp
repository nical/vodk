
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
    virtual SurfaceFormat getFormat() const = 0;
    virtual const byte* getData(int bufIndex = 0) const = 0;
    virtual IntSize getSize() const = 0;
    virtual int getStride() const = 0;
    virtual int getSkip() const = 0;
    virtual ~Surface() {}
};

uint32_t bytesPerPixel(SurfaceFormat format);

} // gfx
} // vodk

#endif
