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

#ifndef VODK_LOGIC_SCOPE_HPP
#define VODK_LOGIC_SCOPE_HPP

#include <vector>
#include <assert.h>

#include "vodk/core/Entity.hpp"
#include "vodk/core/ObjectID.hpp"
#include "vodk/core/Range.hpp"
#include "vodk/core/SubSystem.hpp"

namespace vodk {

class Transform2DSubSystem;
class GfxSubSystem;
class PhysicsSubSystem;
class ControlerSubSystem;
class LogicSubSystem;

namespace gfx {
class Transform;
} // gfx

struct EntityDescriptor {
    Range<SubSystemID> components;
};

class Scope {
public:
    Scope();
    ~Scope();

    EntityID add_entity(EntityDescriptor& desc);

    void remove_entity(EntityID id);

    bool contains_entity(EntityID id);

    void mark_entity_detroyed(EntityID aID);

    SubSystem* get_subsystem(SubSystemID id);

    void add_subsystem(SubSystem* toAdd);

    gfx::Transform& get_transform(EntityID id);

    void resolve_dependencies(); // TODO
protected:
    // the entities are used as a lookup table for the standard components
    std::vector<Entity> _entities;
    SubSystem* _subSystems[SYSTEM_COUNT];
    SubSystemID _scopeID;
    uint16_t _genHash;
};

} // vodk

#endif
