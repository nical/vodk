
#ifndef VODK_CORE_MANAGER_HPP
#define VODK_CORE_MANAGER_HPP

#include <stdint.h>

#include "vodk/core/ObjectID.hpp"
#include "vodk/core/FragmentedArray.hpp"

namespace vodk {

template<typename TObj, typename TObjID, int TChunkSize = 128>
class Manager
{
public:
    typedef TObj ObjectType;
    typedef TObjID ObjectIDType;
    enum { ChunkSize = TChunkSize };
    union MaybeObject {
        ObjectType object;
        ObjectType* freelist;
    }
    typedef FragmentedArray<MaybeObject,ChunkSize> Container;
protected:
    ObjectIDType add(const ObjectType& toCopy)
    {
        Container::ChunkIterator it = _data.chunks();
        while (it.isValid()) {
            auto elts = it.slice();
            for (uint32_t i = 0; i < elts.size(); ++i) {
                // TODO freelist
                if (elts[i].object.state & ENTITY_STATE_EMPTY) {
                    elts[i].object.reset(ENTITY_STATE_NORMAL);
                    return EntityID(i, _genHash++);
                }
            }
            it = it.next();
        }
        _data.pushBack(Entity()).reset(ENTITY_STATE_NORMAL);
        return EntityID(_data.getSize() - 1, _genHash++);
    }

    T* get(ObjectIDType id)
    {
        if (id.index >= _data.getSize()) {
            return nullptr;
        }

        ObjectType* obj = &_data.at(id.index);

        if (obj->genHash != id.genHash) {
            return nullptr;
        }

        return obj;
    }

    uint32_t getSize() const {
        return _data.getSize();
    }

    virtual ~Manager() {}

private:
    Container _data;
    ObjectID::GenHash _genHash;
    uint32_t _size;
};


template<typename TObj, typename TObjID, int TChunkSize = 128>
EntityID Manager::add(const T&)

void Manager::markDestroyed(EntityID id)
{
    Entity* e = _getEntity(id);
    if (e) {
        e->state &= ENTITY_STATE_DESTROYED;
    }
}




} // vodk

#endif
