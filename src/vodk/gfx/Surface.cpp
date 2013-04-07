
#include "vodk/gfx/Surface.hpp"

namespace vodk {
namespace gfx {

uint32_t bytesPerPixel(SurfaceFormat format)
{
    switch (format) {
        case SURFACE_R8G8B8A8: return 4;
        case SURFACE_A8: return 1;
    }
    return 0;
}

} // gfx
} // vodk
