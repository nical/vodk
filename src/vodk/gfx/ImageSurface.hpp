
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
    virtual SurfaceFormat getFormat() const override { return gfx::SURFACE_R8G8B8A8; }
    virtual const byte* getData(int bufIndex = 0) const override { return getPixelsPtr(); }
    virtual IntSize getSize() const override {
        return IntSize(sf::Image::getSize().x, sf::Image::getSize().y);
    }
    virtual int getStride() const override { return getSize().width * 4; } 
    virtual int getSkip() const override { return 0; }
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
    virtual SurfaceFormat getFormat() const override { return gfx::SURFACE_R8G8B8A8; }

    virtual const byte* getData(int bufIndex = 0) const override { return _data; }
    byte* getData(int bufIndex = 0) { return _data; }

    virtual IntSize getSize() const override {
        return _size;
    }
    virtual int getStride() const override { return _stride; } 
    virtual int getSkip() const override { return _skip; }

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
