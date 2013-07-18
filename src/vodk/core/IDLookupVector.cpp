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

#include "vodk/core/IDLookupVector.hpp"

namespace vodk {
namespace debug {

struct IDObj {
    IDObj(int val = 0) : value(val), index(0) {}
    int value;
    uint16_t index;
};

template<>
struct Dump<vodk::debug::IDObj> {
    static void dump(const vodk::debug::IDObj& val) {
        debug::dump<int>(val.value);
    }
};

} // debug

namespace unittest {

using namespace vodk::debug;
void IDLookupVector() {
    vodk::IDLookupVector<IDObj> vec;

    uint16_t idx1 = vec.add(IDObj(1));
    uint16_t idx2 = vec.add(IDObj(2));
    vec.remove(idx1);
    uint16_t idx3 = vec.add(IDObj(3));
    uint16_t idx4 = vec.add(IDObj(4));
    vec.remove(idx3);
    uint16_t idx5 = vec.add(IDObj(5));
    vec.remove(idx2);
    dumpln(vec.get(idx4).value);
    assert(vec.get(idx4).value == 4);
    dumpln(vec.get(idx5).value);
    assert(vec.get(idx5).value == 5);
    debug::dump(vec);
}

} // unittest
} // vodk
