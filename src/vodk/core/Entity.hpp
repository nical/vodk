
#ifndef VODK_CORE_ENTITY_HPP
#define VODK_CORE_ENTITY_HPP

#include "stdint.h"

#define DECLARE_COMPONENT_ID(classname) struct classname : public vodk::ComponentID { \
                                        classname(ComponentID::Handle h = 0, \
                                                  ComponentID::Handle s = 0) \
                                        : ComponentID(s,h) {} \
                                        }; \

namespace vodk {

class Entity;

struct ComponentID {
    typedef uint16_t Handle;

    ComponentID(Handle s, Handle h)
    : system(s), handle(h) {}

    Handle system;
    Handle handle;
};

DECLARE_COMPONENT_ID(GfxComponentID)
DECLARE_COMPONENT_ID(PhysicsComponentID)
DECLARE_COMPONENT_ID(LogicComponentID)


class PluginComponent {
public:
    virtual void update(Entity& e, float dt) = 0;
    virtual void attach(Entity& e, float dt) = 0;
    virtual void detach(Entity& e, float dt) = 0;
    PluginComponent* getNext() { return _next; }
    void setNext(PluginComponent* p) { _next = p; }
private:
    PluginComponent* _next;
};


typedef uint8_t EntityState;
const EntityState ENTITY_STATE_EMPTY        = 0;
const EntityState ENTITY_STATE_NORMAL       = 1 << 0;
const EntityState ENTITY_STATE_DESTROYED    = 1 << 1;


struct EntityID {
    EntityID(uint16_t h, uint8_t gen)
    : handle(h), generation(gen)
    {}

    uint16_t handle;
    uint8_t generation;
};


class Entity {
public:
    typedef uint64_t Family;

    void reset(EntityState state = ENTITY_STATE_NORMAL);

    GfxComponentID      gfxID;
    PhysicsComponentID  physicsID;
    LogicComponentID    logicID;
    Family              family;
    EntityState         state;
    uint8_t             generation;
};

} // vodk


#endif
