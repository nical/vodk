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
#include "vodk/core/Range.hpp"

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

// TODO create entities separately from components
struct EntityDescriptor {
    EntityDescriptor(float x = 0.0f, float y = 0.0f, float z = 0.0f,
                     SubSystemID s1 = 0,
                     SubSystemID s2 = 0,
                     SubSystemID s3 = 0,
                     SubSystemID s4 = 0,
                     SubSystemID s5 = 0,
                     SubSystemID s6 = 0,
                     SubSystemID s7 = 0,
                     SubSystemID s8 = 0)
    : _x(x), _y(y), _z(z)
    , _s1(s1), _s2(s2), _s3(s3), _s4(s4)
    , _s5(s5), _s6(s6), _s7(s7), _s8(s8)
    , components(&_s1, 8)
    {}

    Range<SubSystemID> components;
    float _x, _y, _z;
    SubSystemID _s1;
    SubSystemID _s2;
    SubSystemID _s3;
    SubSystemID _s4;
    SubSystemID _s5;
    SubSystemID _s6;
    SubSystemID _s7;
    SubSystemID _s8;
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
