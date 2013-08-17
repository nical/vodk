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

#ifndef VODK_CORE_FREELISTVECTOR_HPP
#define VODK_CORE_FREELISTVECTOR_HPP

#include <vector>
#include <limits>
#include "vodk/core/Range.hpp"

namespace vodk {

template<typename T>
class FreelistVector {
public:
    typedef uint32_t Offset;
    struct Slot {
        Slot() {}
        Slot(const T& init) {
            new(&padding[0]) T();
            cast() = init;
        }
        T& cast() { return *reinterpret_cast<T*>(this); }
        const T& cast() const { return *reinterpret_cast<const T*>(this); }
        Offset& as_freelist() {
            return *reinterpret_cast<Offset*>(this);
        }
        uint8_t padding[sizeof(T)];
    };

    static const Offset FREELIST_EMPTY = std::numeric_limits<Offset>::max();

    FreelistVector()
    : _free_list(FREELIST_EMPTY)
    {}

    T& operator[](Offset o) {
        return _data[o].cast();
    }

    const T& operator[](Offset o) const {
        return _data[o].cast();
    }

    Offset add(const T& val) {
        if (_free_list != FREELIST_EMPTY) {
            Offset index = _free_list;
            _free_list = _data[_free_list].as_freelist();
            _data[index] = val;
            return index;
        }

        _data.push_back(val);
        return _data.size() - 1;
    }

    void remove(Offset o) {
        _data[o].cast().~T();
        if (_free_list != FREELIST_EMPTY) {
            _data[o].as_freelist() = _free_list;
            _free_list = o;
        }
    }

    Offset push_back(const T& val) {
        _data.push_back(val);
        return size() -1;
    }

    void reserve(Offset size) {
        _data.reserve(size);
    }

    Offset size() const {
        return _data.size();
    }

    Range<T> range() {
        return Range<T>(&_data[0].data, _data.size());
    }

    Range<const T> const_range() const {
        return Range<const T>(&_data[0].data, _data.size());
    }

    void clear() {
        _data.clear();
    }

private:
    std::vector<Slot> _data;
    Offset _free_list;
};

} // vodk

#endif
