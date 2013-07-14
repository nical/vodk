#ifndef VODK_CORE_SUBSYSTEM_HPP
#define VODK_CORE_SUBSYSTEM_HPP

#include <stdint.h>
#include "vodk/core/IDLookupVector.hpp"
#include "vodk/core/ObjectID.hpp"

namespace vodk {

class SubSystem {
public:
    typedef ObjectID::SubSystem ID;

    virtual ~SubSystem() {}
    virtual void update(float dt) = 0;
    virtual void flush() = 0;
    virtual void destroy() = 0;
    virtual ID id() const = 0;
};

template <typename T, typename ObjID>
class TSubSystem : public SubSystem
{
public:
    TSubSystem(SubSystem::ID aID, uint32_t preallocate = 64)
    : _objects(preallocate)
    , _id(aID)
    , _genHash(0)
    {}

    virtual SubSystem::ID id() const override { return _id; }

    uint32_t size() const { return _objects.size(); }

    bool contains(ObjID e) const {
        bool result = _objects.contains(e.index);
        return result && e.genHash == _objects.get(e.index).genHash;
    }

    T& get(ObjID e) {
        T& result = _objects.get(e.index);
        assert(e.genHash == result.genHash);
        return result;
    }

    ObjID add(T& e) {
        e.genHash == _genHash++;
        return ObjID(_objects.add(e), id(), e.genHash);
    }

    void remove(ObjID e) {
        assert(contains(e));
        _objects.remove(e.index);
    }

protected:
    IDLookupVector<T, typename ObjID::Index> _objects;
    SubSystem::ID _id;
    ObjectID::GenHash _genHash;
};



} // vodk

#endif
