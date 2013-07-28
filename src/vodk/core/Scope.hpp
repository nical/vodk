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
#include <functional>

#include "vodk/core/Entity.hpp"
#include "vodk/core/ObjectID.hpp"
#include "vodk/core/Range.hpp"
#include "vodk/gfx/Transform.hpp"

namespace vodk {


enum SubSystemName {
    SYSTEM_NONE,
    // transforms
    SYSTEM_TRANFORM,
    // gfx/anim
    SYSTEM_ANIMATION_SPRITE,
    SYSTEM_ANIMATION_SKELETON,
    // gfx
    SYSTEM_GFX_BASIC,
    SYSTEM_GFX_QUAD,
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

class Transform2DSubSystem;
class SubSystem;
class GfxSubSystem;
class PhysicsSubSystem;
class ControlerSubSystem;
class LogicSubSystem;
class Scope;

namespace gfx {
class Transform;
} // gfx

typedef std::function<void(Scope* s, Entity* e, EntityID e_id)> EntityInitCB;


typedef uint8_t EntityState;
const EntityState ENTITY_STATE_EMPTY        = 0;
const EntityState ENTITY_STATE_NORMAL       = 1 << 0;

enum ComponentCategory {
    COMPONENT_PHYSICS,
    COMPONENT_GRAPHICS,
    COMPONENT_ANIM,
    COMPONENT_CONTROLER,
    COMPONENT_PLUGIN,
};

struct EntityDescriptor {
    EntityDescriptor(float x, float y, float z,
                     SubSystemID a_gfx, SubSystemID a_anim,
                     SubSystemID a_physics, SubSystemID a_controler,
                     EntityInitCB a_cb = nullptr)
    : transform(x,y,z)
    , graphics(a_gfx)
    , anim(a_anim)
    , physics(a_physics)
    , controler(a_controler)
    , callback(a_cb)
    {}

    gfx::Transform transform;
    SubSystemID graphics;
    SubSystemID anim;
    SubSystemID physics;
    SubSystemID controler;
    std::vector<SubSystemID> plugin_components;
    EntityInitCB callback;
};

struct Entity {
    static const int MAX_COMPONENTS = 8;


    ComponentID components[MAX_COMPONENTS];

    Range<ComponentID> standard_components() { return range(&_graphics, 4); }
    Range<ComponentID> plugin_components() {
        return range(_plugin_components.data(),
                     _plugin_components.size());
    }

    ComponentID* find_plugin_component(SubSystemID id);

    ComponentID get_graphics_component() const { return _graphics; }
    ComponentID get_physics_component() const { return _physics; }
    ComponentID get_anim_component() const { return _anim; }
    ComponentID get_controler_component() const { return _controler; }
    EntityState get_state() const { return _state; }
    gfx::Transform* get_transform() { return &_transform; }

    gfx::Transform _transform;
    ComponentID _graphics;
    ComponentID _anim;
    ComponentID _physics;
    ComponentID _controler;
    std::vector<ComponentID> _plugin_components;
    EntityState _state;
    uint16_t    _gen;
};

class Scope {
public:
    Scope();
    ~Scope();

    // takes effect when flush() is called
    void add_entity_async(const EntityDescriptor& desc);

    // takes effect when flush() is called
    void remove_entity_async(EntityID id);

    // resolve added and removed entities
    void flush();

    bool contains_entity(EntityID id);

    SubSystem* get_subsystem(SubSystemID id);

    void add_subsystem(SubSystem* toAdd);

    gfx::Transform* get_transform(EntityID id);

    void resolve_dependencies(); // TODO

protected:
    Range<Entity> entity_range();

    void init_entity(Entity& e, EntityOffset o, EntityDescriptor& desc);
    void reset_entity(Entity& e);

    // the entities are used as a lookup table for the standard components
    std::vector<Entity> _entities;
    SubSystem* _subSystems[SYSTEM_COUNT];
    SubSystemID _scopeID;
    uint16_t _genHash;
    std::vector<EntityDescriptor> _to_add;
    std::vector<EntityOffset> _to_remove;
};

} // vodk

#endif
