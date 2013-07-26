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

#include "vodk/core/Scope.hpp"
#include <memory.h>

namespace vodk {

EntityID Scope::add_entity(const EntityDescriptor& desc) {
    EntityOffset idx = 0;

    while (idx != _entities.size() && _entities[idx].state != ENTITY_STATE_NORMAL) {
        ++idx;
    }

    ++_genHash;

    if (idx == _entities.size()) {
        _entities.push_back(Entity());
    }

    Entity& e = _entities[idx];
    e.state = ENTITY_STATE_NORMAL;
    e.genHash = _genHash;
    for (unsigned i = 0; i < desc.components.size(); ++i) {
        e.components[i].subSystem = desc.components[i];
        e.components[i].offset = get_subsystem(desc.components[i])->add(idx);
    }

    return EntityID(idx, _scopeID);
}

void Scope::mark_entity_detroyed(EntityID aID) {
    assert(contains_entity(aID));
    _entities[aID.offset].state = ENTITY_STATE_DESTROYED;
}

void Scope::remove_entity(EntityID aID) {
    assert(contains_entity(aID));

    Entity& e = _entities[aID.offset];

    for (int i = 0; i < SYSTEM_COUNT; ++i) {
        for (int j = 0; j < Entity::MAX_COMPONENTS; ++j) {
            if (e.components[j].subSystem == _subSystems[i]->id()) {
                _subSystems[i]->remove(e.components[i].offset,
                                       Range<Entity>(_entities.data(),
                                                     _entities.size()));
            }
        }
    }
}

bool Scope::contains_entity(EntityID id) {
    return id.offset < _entities.size()
           && _entities[id.offset].state == ENTITY_STATE_NORMAL;
}

gfx::Transform& Scope::get_transform(EntityID id) {
    _entities[id.offset].transform;
}

void Scope::add_subsystem(SubSystem* toAdd) {
    assert(toAdd);
    assert(!_subSystems[toAdd->id()]);
    _subSystems[toAdd->id()] = toAdd;
}

SubSystem* Scope::get_subsystem(SubSystemID id) {
    assert(id < SYSTEM_COUNT);
    return _subSystems[id];
}

Scope::Scope() {
    memset(_subSystems, 0, SYSTEM_COUNT * sizeof(SubSystem*));
}

Scope::~Scope() {
    for (unsigned i = 0; i < SYSTEM_COUNT; ++i) {
        _subSystems[i]->destroy();
        delete _subSystems[i];        
    }
}

} // vodk
