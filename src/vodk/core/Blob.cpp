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

#include "vodk/core/Blob.hpp"

namespace vodk {

struct BlobMetaData {
	uint32_t size;
	uint32_t type;
	uint32_t offset;
};

static BlobMetaData* _md(void* buf) {
	return static_cast<BlobMetaData*>(buf);
}

static uint8_t* _data(void* buf) {
	return static_cast<uint8_t*>(buf) + static_cast<BlobMetaData*>(buf)->offset;
}

ByteRange Blob::bytes()
{
	return ByteRange(_data(_buffer), _md(_buffer)->size);
}

uint32_t Blob::size() const
{
	return _md(_buffer)->size;
}

uint32_t Blob::type() const
{
	return _md(_buffer)->type;
}


bool Blob::allocate(uint32_t size, uint32_t type) {
	_buffer = new uint8_t[size + sizeof(BlobMetaData)];
	_md(_buffer)->size = size;
	_md(_buffer)->type = type;
	_md(_buffer)->offset = sizeof(BlobMetaData);
	return true;
}

void Blob::deallocate() {
	delete (uint8_t*)_buffer;
}

} // vodk
