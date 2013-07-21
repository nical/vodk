

#include "vodk/parser/png.hpp"
#include "vodk/gfx/ImageSurface.hpp"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <png.h>

namespace vodk {
namespace parser {

struct ScopedCloseFile {
    ScopedCloseFile(FILE* fp)
    : _fp(fp) {}
    ~ScopedCloseFile() {
        if (_fp) {
            fclose(_fp);
        }
    }
    FILE* _fp;
};

PNGImage::PNGImage()
: _rows(nullptr)
, _png_ptr(nullptr)
, _info_ptr(nullptr)
{

}

PNGImage::~PNGImage()
{
    if (_rows) {
        for (int y = 0; y < _size.height; y++) {
            free(_rows[y]);
        }
        free(_rows);
    }
}

png_structp png(void* ptr) { return static_cast<png_structp>(ptr); }
png_infop png_info(void* ptr) { return static_cast<png_infop>(ptr); }

bool PNGImage::load_from_file(const char* path)
{
    uint8_t header[8];    // 8 is the maximum size that can be checked

    // open file and test for it being a png
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        printf("[read_png_file] File %s could not be opened for reading", path);
        return false;
    }

    ScopedCloseFile scope_close(fp);

    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8)) {
        printf("[read_png_file] File %s is not recognized as a PNG file", path);
        return false;
    }

    // initialize stuff
    _png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

    if (!_png_ptr) {
        printf("[read_png_file] png_create_read_struct failed");
        return false;
    }

    _info_ptr = png_create_info_struct(png(_png_ptr));
    if (!_info_ptr) {
        printf("[read_png_file] png_create_info_struct failed");
        return false;
    }

    if (setjmp(png_jmpbuf(png(_png_ptr)))) {
        printf("[read_png_file] Error during init_io");
        return false;
    }

    png_init_io(png(_png_ptr), fp);
    png_set_sig_bytes(png(_png_ptr), 8);

    png_read_info(png(_png_ptr), png_info(_info_ptr));

    _size.width = png_get_image_width(png(_png_ptr), png_info(_info_ptr));
    _size.height = png_get_image_height(png(_png_ptr), png_info(_info_ptr));
    png_byte color_type = png_get_color_type(png(_png_ptr), png_info(_info_ptr));

    int bit_depth = png_get_bit_depth(png(_png_ptr), png_info(_info_ptr));
    // let's just support 8bits per channel for now
    if (bit_depth == 16) {
        png_set_strip_16(png(_png_ptr));
    }
    if (bit_depth < 8) {
        png_set_packing(png(_png_ptr));
    }
    // fill alpha channel with 255 if absent
    if (color_type == PNG_COLOR_TYPE_RGB){
        png_set_filler(png(_png_ptr), 255, PNG_FILLER_BEFORE);
    }

    int number_of_passes = png_set_interlace_handling(png(_png_ptr));
    png_read_update_info(png(_png_ptr), png_info(_info_ptr));

    // read file
    if (setjmp(png_jmpbuf(png(_png_ptr)))) {
        printf("[read_png_file] Error during read_image");
        return false;
    }

    _rows = (png_bytep*) malloc(sizeof(png_bytep) * _size.height);
    for (int y = 0; y < _size.height; y++) {
        _rows[y] = (png_byte*) malloc(png_get_rowbytes(png(_png_ptr),png_info(_info_ptr)));
    }

    png_read_image(png(_png_ptr), _rows);

    return true;
}

bool PNGImage::copy_into(gfx::ImageSurface& surface)
{
    surface.allocate(_size, gfx::SURFACE_R8G8B8A8);
    int bpp = gfx::bytes_per_pixel(gfx::SURFACE_R8G8B8A8);
    uint8_t* surf_row = surface.get_data(); 
    for (int i = 0; i < _size.height; ++i) {
        memcpy(surf_row, _rows[i], _size.width * bpp);
        surf_row += surface.get_stride();
    }
}

} // parser
} // vodk
