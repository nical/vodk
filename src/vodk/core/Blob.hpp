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

class Blob {
public:
    Blob() : _buffer(nullptr) {}

    ByteRange bytes();

    template<typename T> Range<T> range() {
        return Range<T>((T*)bytes().pointer(), size()/sizeof(T));
    }

    uint32_t size() const;
    uint32_t type() const;

    bool is_allocated() const { return !!_buffer; }

    bool allocate(uint32_t size, uint32_t type = 0);
    void deallocate();

protected:
    void* _buffer;
};

struct AutoDeallocateBlob {
    AutoDeallocateBlob(Blob& b) : blob(&b) {}
    ~AutoDeallocateBlob() { blob->deallocate(); }
    Blob* blob;
};

} // vodk

#endif
