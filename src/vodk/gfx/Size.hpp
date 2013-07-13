
#ifndef VODK_GFX_SIZE_HPP
#define VODK_GFX_SIZE_HPP

namespace vodk {
namespace gfx {

template<typename T>
struct Size {
    Size(T w, T h) : width(w), height(h) {}
    T width;
    T height;
};

struct IntSize : public Size<int> {
    typedef int ValueType;
    typedef Size<int> Parent;

    IntSize(int w, int h) : Parent(w,h) {}
    IntSize() : Parent(0,0) {}
    bool operator == (const IntSize& other) {
        return width == other.width && height == other.height;
    }
};

} // namespace
} // namespace

#endif
