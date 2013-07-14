
#ifndef VODK_CORE_ENTITY_HPP
#define VODK_CORE_ENTITY_HPP

#include <stdint.h>
#include "vodk/core/ObjectID.hpp"
#include "vodk/core/SubSystem.hpp"

#define DECLARE_ID(classname) struct classname : public vodk::ObjectID { \
                                  classname(ObjectID::Index i = 0, \
                                            ObjectID::SubSystem s = 0, \
                                            ObjectID::GenHash g = 0) \
                                  : ObjectID(i,s,g) {} \
                              }; \

namespace vodk {

class Entity;

DECLARE_ID(EntityID)
DECLARE_ID(GfxComponentID)
DECLARE_ID(PhysicsComponentID)
DECLARE_ID(LogicComponentID)
#undef DECLARE_ID

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
const EntityState ENTITY_STATE_NORMAL       = 0;
const EntityState ENTITY_STATE_DESTROYED    = 1 << 1;

class Entity {
public:
    typedef uint64_t Family;

    void reset(EntityState state = ENTITY_STATE_NORMAL);

    GfxComponentID      gfxID;
    PhysicsComponentID  physicsID;
    LogicComponentID    logicID;
    Family              family;
    EntityState         state;
    ObjectID::GenHash   genHash;
    ObjectID::Index     index;
};

class EntitySubSystem : TSubSystem<Entity, EntityID>
{
public:
    typedef TSubSystem<Entity, EntityID> Parent;
    EntitySubSystem(SubSystem::ID aID, uint32_t preallocate = 64)
    : Parent(aID, preallocate)
    {}

    virtual void update(float dt) override {}

    virtual void flush() override {}

    virtual void destroy() override {}
};

} // vodk

#endif
