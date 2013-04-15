
#include "vodk/core/Entity.hpp"

namespace vodk {

void Entity::reset(EntityState s)
{
    gfxID = GfxComponentID();
    physicsID = PhysicsComponentID();
    logicID = LogicComponentID();
    family = 0;
    state = s;
}

} // vodk
