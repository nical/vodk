
#ifndef VODK_CORE_ENTITY_HPP
#define VODK_CORE_ENTITY_HPP

#include "stdint.h"
#include "vodk/core/ObjectID.hpp"

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
};

} // vodk


#endif
