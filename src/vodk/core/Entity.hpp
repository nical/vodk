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

#ifndef VODK_CORE_ENTITY_HPP
#define VODK_CORE_ENTITY_HPP

#include <stdint.h>
#include "vodk/core/ObjectID.hpp"
#include "vodk/gfx/Transform.hpp"

namespace vodk {

class Entity;

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

struct Entity {
    static const int MAX_COMPONENTS = 8;

    gfx::Transform transform;

    ComponentID components[MAX_COMPONENTS];
    EntityState state;
    uint16_t genHash;

    ComponentID* getComponent(SubSystemID system) {
        for (unsigned i = 0; i < MAX_COMPONENTS; ++i) {
            if (components[i].subSystem == system) {
                return &components[i];
            }
        }
        return nullptr;
    }
};

} // vodk

#endif
