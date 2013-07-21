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

#ifndef VODK_CORE_SUBSYSTEM_HPP
#define VODK_CORE_SUBSYSTEM_HPP

#include <stdint.h>
#include "vodk/core/IDLookupVector.hpp"
#include "vodk/core/ObjectID.hpp"
#include "vodk/core/Entity.hpp"
#include "vodk/core/Range.hpp"

namespace vodk {

struct SubSystem {
    virtual ~SubSystem() {}
    virtual SubSystemID id() const = 0;

    virtual ComponentOffset add(EntityOffset e) = 0;
    virtual void remove(ComponentOffset, Range<Entity> entities) = 0;

    virtual void destroy() {}

    virtual void update(float dt) {}
    virtual void flush() {};

    virtual Range<SubSystemID> dependencies() = 0;
};

template<typename T>
struct TSubSystem : public SubSystem {
    TSubSystem(SubSystemID aID, ComponentOffset preallocate = 128)
    : _subSystemID(aID)
    {
        _components.reserve(preallocate);
    }

    virtual ~TSubSystem() {}

    virtual ComponentOffset add(EntityOffset entity) override {
        _components.resize(_components.size() + 1);
        return _components.size() -1;
    }

    virtual void remove(ComponentOffset offset, Range<Entity> entities) override {
        // swap the remvoed element with the last one
        _components[offset] = _components[_components.size() - 1];
        // fixup the offset in the entity that was pointing to the last one
        entities[_components[offset].entity].getComponent(_subSystemID)->offset = offset;
        // resize
        _components.resize(_components.size() - 1);
    }

    virtual SubSystemID id() const {return _subSystemID; }

    Range<T> range() { return Range<T>(_components.data(), _components.size()); }

protected:
    std::vector<T>  _components;
    SubSystemID     _subSystemID;
};

enum SubSystemName {
    // transforms
    SYSTEM_TRANFORM_2D,
    SYSTEM_TRANFORM_3D,
    // gfx/anim
    SYSTEM_ANIMATION_SPRITE,
    SYSTEM_ANIMATION_SKELETON,
    // gfx
    SYSTEM_GFX_TILEDMAP,
    SYSTEM_GFX_IMAGE,
    SYSTEM_GFX_3DMODEL,
    SYSTEM_GFX_PARTICLE_0,
    SYSTEM_GFX_PARTICLE_1,
    SYSTEM_GFX_PARTICLE_2,
    SYSTEM_GFX_PARTICLE_3,
    SYSTEM_GFX_PARTICLE_4,
    SYSTEM_GFX_PARTICLE_5,
    SYSTEM_GFX_PARTICLE_6,
    SYSTEM_GFX_PARTICLE_7,
    SYSTEM_GFX_PARTICLE_8,
    SYSTEM_GFX_PARTICLE_9,
    SYSTEM_GFX_PARTICLE_10,
    SYSTEM_GFX_PARTICLE_11,
    SYSTEM_GFX_PARTICLE_12,
    SYSTEM_GFX_PARTICLE_13,
    SYSTEM_GFX_PARTICLE_14,
    SYSTEM_GFX_PARTICLE_15,
    // collisions
    SYSTEM_COLLISION_RAYCAST,
    SYSTEM_COLLISION_SENSOR,
    SYSTEM_COLLISION_BBOX,
    SYSTEM_COLLISION_BSPHERE,

    // type classes
    SYSTEM_LOGIC_TYPE_0,
    SYSTEM_LOGIC_TYPE_1,
    SYSTEM_LOGIC_TYPE_2,
    SYSTEM_LOGIC_TYPE_3,
    SYSTEM_LOGIC_TYPE_4,
    SYSTEM_LOGIC_TYPE_5,
    SYSTEM_LOGIC_TYPE_6,
    SYSTEM_LOGIC_TYPE_7,
    SYSTEM_LOGIC_TYPE_8,
    SYSTEM_LOGIC_TYPE_9,
    SYSTEM_LOGIC_TYPE_10,
    SYSTEM_LOGIC_TYPE_11,
    SYSTEM_LOGIC_TYPE_12,
    SYSTEM_LOGIC_TYPE_13,
    SYSTEM_LOGIC_TYPE_14,
    SYSTEM_LOGIC_TYPE_15,
    SYSTEM_LOGIC_TYPE_16,
    SYSTEM_LOGIC_TYPE_17,
    SYSTEM_LOGIC_TYPE_18,
    SYSTEM_LOGIC_TYPE_19,
    SYSTEM_LOGIC_TYPE_20,
    SYSTEM_LOGIC_TYPE_21,
    SYSTEM_LOGIC_TYPE_22,
    SYSTEM_LOGIC_TYPE_23,
    SYSTEM_LOGIC_TYPE_24,
    SYSTEM_LOGIC_TYPE_25,
    SYSTEM_LOGIC_TYPE_26,
    SYSTEM_LOGIC_TYPE_27,
    SYSTEM_LOGIC_TYPE_28,
    SYSTEM_LOGIC_TYPE_29,
    SYSTEM_LOGIC_TYPE_30,
    SYSTEM_LOGIC_TYPE_31,

    SYSTEM_LOGIC_LUA,
    SYSTEM_LOGIC_KIWI,
    SYSTEM_LOGIC_DECLARATIVE,
    SYSTEM_LOGIC_FOLLOW,
    SYSTEM_LOGIC_BULLET,
    SYSTEM_LOGIC_CINEMATIC,
    SYSTEM_LOGIC_TIMER,

    SYSTEM_PHYSICS_BOX2D,
    SYSTEM_PHYSICS_BASIC,

    SYSTEM_COUNT // sentinel
};

/*

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

    Range<T> range() { return _objects.range(); }

protected:
    void remove(ObjID e) {
        assert(contains(e));
        _objects.remove(e.index);
    }

    IDLookupVector<T, typename ObjID::Index> _objects;
    SubSystem::ID _id;
    ObjectID::GenHash _genHash;
};
*/


} // vodk

#endif
