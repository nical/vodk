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

#ifndef VODK_CORE_OBJECTID_HPP
#define VODK_CORE_OBJECTID_HPP

#include <stdint.h>

namespace vodk {

typedef uint8_t SubSystemID;
typedef uint8_t SubSystemOffset;
typedef uint16_t ComponentOffset;
typedef uint16_t EntityOffset;

struct ComponentID {
    ComponentID(ComponentOffset o = 0, SubSystemID s = 0)
    : offset(o), subSystem(s) {}

    ComponentOffset offset;
    SubSystemID subSystem;
};

struct EntityID {
    EntityID(EntityOffset o = 0, SubSystemID s = 0)
    : offset(o), scope(s) {}

    EntityOffset offset;
    SubSystemID scope;
};

template<typename I, typename S, typename G>
struct _ObjectID {
    typedef I Index;
    typedef S SubSystem;
    typedef G GenHash;

    _ObjectID(Index i, SubSystem s, GenHash g)
    : index(i), subSystem(s), genHash(g) {}

    Index index;
    SubSystem subSystem;
    GenHash genHash;
};

typedef _ObjectID<uint16_t, uint8_t, uint8_t> ObjectID;

} // vodk

#endif
