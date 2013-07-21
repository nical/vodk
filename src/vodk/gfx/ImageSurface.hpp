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

#ifndef VODK_GFX_IMAGESURFACE_HPP
#define VODK_GFX_IMAGESURFACE_HPP

#ifdef VODK_WIDGET_SFML
#include <SFML/Graphics/Image.hpp>
#endif // SFML

#include "vodk/gfx/Surface.hpp"

namespace vodk {
namespace gfx {

#ifdef VODK_WIDGET_SFML
class ImageSurfaceSFML : public sf::Image
                       , public gfx::Surface
{
public:

    // Surface
    virtual SurfaceFormat get_format() const override { return gfx::SURFACE_R8G8B8A8; }
    virtual const byte* get_data(int bufIndex = 0) const override { return getPixelsPtr(); }
    virtual IntSize get_size() const override {
        return IntSize(sf::Image::get_size().x, sf::Image::get_size().y);
    }
    virtual int get_stride() const override { return get_size().width * 4; } 
    virtual int get_skip() const override { return 0; }
};

#endif

class ImageSurface : public gfx::Surface
{
public:
    typedef uint8_t byte;
    ImageSurface()
    : _data(nullptr), _stride(0), _skip(0), _format(gfx::SURFACE_R8G8B8A8)
    {}

    ~ImageSurface() {
        deallocate();
    }

    bool allocate(const IntSize& size, SurfaceFormat f);
    void deallocate();

    // Surface
    virtual SurfaceFormat get_format() const override { return gfx::SURFACE_R8G8B8A8; }

    virtual const byte* get_data(int bufIndex = 0) const override { return _data; }
    byte* get_data(int bufIndex = 0) { return _data; }

    virtual IntSize get_size() const override {
        return _size;
    }
    virtual int get_stride() const override { return _stride; } 
    virtual int get_skip() const override { return _skip; }

protected:
    byte* _data;
    IntSize _size;
    uint32_t _stride;
    uint32_t _skip;
    SurfaceFormat _format;
};

} // gfx
} // vodk

#endif
