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

#ifndef VODK_CORE_BLOB_HPP
#define VODK_CORE_BLOB_HPP

#include <stdint.h>
#include "vodk/core/Range.hpp"

namespace vodk {

struct TemporaryBlob {
	TemporaryBlob(void* buf) : _buffer(buf) {}
	void* _buffer;
};

class Blob {
public:
    Blob() : _buffer(nullptr) {}

    Blob(TemporaryBlob tb) : _buffer(tb._buffer) {}

    ~Blob() { deallocate(); }

    uint32_t size() const;

    uint32_t type() const;

    bool is_allocated() const { return !!_buffer; }

    bool allocate(uint32_t size, uint32_t type = 0);

    void deallocate();

    bool clone_into(Blob& into) const;

    bool copy_into(ByteRange& into) const;

    ByteRange bytes();

    uint8_t* data();

    Range<const uint8_t> const_bytes() const;

    template<typename T> Range<T> range() {
        return Range<T>((T*)bytes().pointer(), size()/sizeof(T));
    }

    template<typename T> Range<const T> const_range() {
        return Range<T>((const T*)bytes().pointer(), size()/sizeof(T));
    }

    /**
     * Loose ownership and reference to the data.
     */
    TemporaryBlob drop() {
    	return TemporaryBlob(_buffer);
    	_buffer = nullptr;
    }

protected:
    void* _buffer;
};

class StackBlob : public Blob {
public:
    template<typename T>
    T* push_back(const T& value) {
        if (_stack_offset + sizeof(T) >= size()) {
            return nullptr;
        }
        T* ptr = static_cast<T*>(data()+ _stack_offset);
        *ptr = value;
        _stack_offset += sizeof(T); 
        return ptr;
    }
protected:
    uint32_t _stack_offset;
    uint32_t _size;
};

template<typename T, uint32_t N>
struct Array {
    T& operator[](uint32_t offset) {
        assert(offset < N);
        return _data[offset];
    }

    const T& operator[](uint32_t offset) const {
        assert(offset < N);
        return _data[offset];
    }
    T _data[N];
};

} // vodk

#endif
