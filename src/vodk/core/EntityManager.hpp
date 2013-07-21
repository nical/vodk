
#ifndef VODK_CORE_ENTITYMANAGER_HPP
#define VODK_CORE_ENTITYMANAGER_HPP

#include "vodk/core/FragmentedArray.hpp"
#include "vodk/core/Entity.hpp"
#include <stdint.h>

namespace vodk {
/*
class EntityManager
{
public:
    EntityID add_entity();
    const Entity* getEntity(EntityID id);
    void markDestroyed(EntityID id);
    uint32_t get_numberOfEntities() const;
    uint32_t getAllocatedSize() const; // in bytes

    void applyDestroyed();
private:
    Entity* _getEntity(EntityID id);

    typedef FragmentedArray<Entity,128> EntityContainer;
    EntityContainer _entities;
    ObjectID::GenHash _genHash;
    uint32_t _size;
};
*/
} // vodk

#endif
