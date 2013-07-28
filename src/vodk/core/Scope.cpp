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
#include "vodk/core/SubSystem.hpp"
#include <memory.h>

namespace vodk {

Range<Entity> Scope::entity_range() {
    return range(_entities.data(), _entities.size());
}

void Scope::add_entity_async(const EntityDescriptor& desc) {
    _to_add.push_back(desc);
}

void Scope::remove_entity_async(EntityID aID) {
    assert(contains_entity(aID));
    _entities[aID.offset]._state = ENTITY_STATE_EMPTY;
}

bool Scope::contains_entity(EntityID id) {
    return id.offset < _entities.size()
           && _entities[id.offset].get_state() == ENTITY_STATE_NORMAL;
}

void Scope::flush() {
    auto it = _entities.begin();
    auto stop = _entities.end();
    auto add_it = _to_add.begin();
    auto add_stop = _to_add.end();
    // iterate over all entities
    while (it != stop) {
        if (it->get_state() == ENTITY_STATE_NORMAL) {
            continue;
        }

        // iterate over standard components to remove them
        auto c_it = it->standard_components().begin();
        auto c_stop = it->standard_components().end();
        while (c_it != c_stop) {
            if (c_it->subSystem == 0) continue;
            _subSystems[c_it->subSystem]->remove(c_it->offset, entity_range());
            ++c_it;
        }

        // iterate over plugin components to remove them
        c_it = it->plugin_components().begin();
        c_stop = it->plugin_components().end();
        while (c_it != c_stop) {
            if (c_it->subSystem == 0) continue;
            _subSystems[c_it->subSystem]->remove(c_it->offset, entity_range());
            ++c_it;
        }

        // if there are objects to add, add one in the blank we just made.
        if (add_it != add_stop) {
            EntityOffset offset = &(*it) - &(*_entities.begin());
            init_entity(*it, offset, *add_it);
            ++add_it;
        } else {
            reset_entity(*it);
        }
        ++it;
    }

    // process the remaining entities to add
    while (add_it != add_stop) {
        _entities.push_back(Entity());
        EntityOffset offset = _entities.size() - 1;
        init_entity(_entities[offset], offset, *add_it);
        ++add_it;
    }
    _to_add.clear();
}

void Scope::init_entity(Entity& e, EntityOffset offset, EntityDescriptor& desc)
{
    e._transform = desc.transform;
    e._physics = ComponentID(desc.physics, desc.physics ? get_subsystem(desc.physics)->add(offset) : 0);
    e._graphics = ComponentID(desc.graphics, desc.graphics ? get_subsystem(desc.graphics)->add(offset) : 0);
    e._anim = ComponentID(desc.anim, desc.anim ? get_subsystem(desc.anim)->add(offset) : 0);
    e._controler = ComponentID(desc.controler, desc.controler ? get_subsystem(desc.controler)->add(offset) : 0);
    auto it = desc.plugin_components.begin();
    auto stop = desc.plugin_components.end();
    e._plugin_components.reserve(desc.plugin_components.size());
    while (it != stop) {
        e._plugin_components.push_back(ComponentID(*it, get_subsystem(*it)->add(offset)));
        ++it;
    }
}

void Scope::reset_entity(Entity& e) {
    e._state = ENTITY_STATE_EMPTY;
    e._plugin_components.clear();
};

ComponentID*
Entity::find_plugin_component(SubSystemID id) {
    auto it = plugin_components().begin();
    auto stop = plugin_components().end();
    while (it != stop) {
        if (it->subSystem == id) {
            return it;
        }
    }
    return nullptr;
}


gfx::Transform& Scope::get_transform(EntityID id) {
    _entities[id.offset]._transform;
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
