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
