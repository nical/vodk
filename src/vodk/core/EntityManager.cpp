
#include "vodk/core/EntityManager.hpp"

namespace vodk {
/*
EntityID EntityManager::add_entity()
{
    EntityContainer::ChunkIterator it = _entities.chunks();
    while (it.is_valid()) {
        auto elts = it.slice();
        for (uint32_t i = 0; i < elts.size(); ++i) {
            if (elts[i].state & ENTITY_STATE_EMPTY) {
                elts[i].reset(ENTITY_STATE_NORMAL);
                return EntityID(i, _genHash++);
            }
        }
        it = it.next();
    }
    _entities.pushBack(Entity()).reset(ENTITY_STATE_NORMAL);
    return EntityID(_entities.get_size() - 1, _genHash++);
}

const Entity* EntityManager::getEntity(EntityID id)
{
    Entity* e = _getEntity(id); // non const, protected
    return e;
}

Entity* EntityManager::_getEntity(EntityID id)
{
    if (id.index >= _entities.get_size()) {
        return nullptr;
    }

    Entity* e = &_entities.at(id.index);

    if (e->genHash != id.genHash) {
        return nullptr;
    }

    return e;
}

void EntityManager::markDestroyed(EntityID id)
{
    Entity* e = _getEntity(id);
    if (e) {
        e->state &= ENTITY_STATE_DESTROYED;
    }
}

void EntityManager::applyDestroyed()
{
    EntityContainer::ChunkIterator it = _entities.chunks();
    while (it.is_valid()) {
        auto elts = it.slice();
        for (uint32_t i = 0; i < elts.size(); ++i) {
            if (elts[i].state & ENTITY_STATE_DESTROYED) {
                // TODO: go through components and remove
                elts[i].reset(ENTITY_STATE_EMPTY);
            }
        }
        it = it.next();
    }
}

uint32_t EntityManager::get_numberOfEntities() const
{
    return _entities.get_size();
}

uint32_t EntityManager::getAllocatedSize() const
{
    return sizeof(this) + _entities.getCapacity() * sizeof(Entity);
}
*/
} // vodk