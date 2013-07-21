
#ifndef VODK_PARSER_PNG_HPP
#define VODK_PARSER_PNG_HPP

#include <stdint.h>
#include "vodk/gfx/Size.hpp"

namespace vodk {
namespace gfx {
class ImageSurface;
} // gfx
} // vodk

namespace vodk {
namespace parser {

class PNGImage {
public:
    PNGImage();
    ~PNGImage();

    bool load_from_file(const char* path);
    bool save_to_file(const char* path);

    gfx::IntSize get_size() const {
        return _size;
    }

    bool copy_into(gfx::ImageSurface& surface);

protected:
    gfx::IntSize _size;
    uint8_t** _rows;
    void* _png_ptr;
    void* _info_ptr;
};


} // parser
} // png

#endif
