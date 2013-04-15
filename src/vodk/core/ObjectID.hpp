
#ifndef VODK_CORE_OBJECTID_HPP
#define VODK_CORE_OBJECTID_HPP

#include <stdint.h>

namespace vodk {

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
