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
#include "vodk/core/Scope.hpp"
#include "vodk/core/Range.hpp"

namespace vodk {

struct SubSystem {
    virtual ~SubSystem() {}
    virtual SubSystemID id() const = 0;

    virtual ComponentOffset add(EntityID id) = 0;
    virtual void remove(ComponentOffset, Range<Entity> entities) = 0;

    virtual void destroy() {}

    virtual void update(float dt) {}
    virtual void flush() {};

    //virtual Range<SubSystemID> dependencies() = 0;
};

template<typename T, ComponentCategory Category = COMPONENT_PLUGIN>
struct TSubSystem : public SubSystem {
    TSubSystem(ComponentOffset preallocate = 128)
    {
        _components.reserve(preallocate);
    }

    virtual ~TSubSystem() {}

    virtual ComponentOffset add(EntityID entity) override {
        _components.resize(_components.size() + 1);
        _components[_components.size() - 1].entity = entity;
        return _components.size() -1;
    }

    virtual void remove(ComponentOffset offset, Range<Entity> entities) override {
        // swap the remvoed element with the last one
        _components[offset] = _components[_components.size() - 1];
        // fixup the offset in the entity that was pointing to the last one
        if (Category == COMPONENT_PLUGIN) {
            entities[_components[offset].entity.offset].find_plugin_component(id())->offset = offset;
        } else {
            entities[_components[offset].entity.offset].standard_components()[Category].offset = offset;
        }
        // resize
        _components.resize(_components.size() - 1);
    }

    Range<T> range() { return Range<T>(_components.data(), _components.size()); }

protected:
    std::vector<T>  _components;
};

} // vodk

#endif
