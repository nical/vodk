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

#ifndef VODK_DATA_RESOURCEMANAGER_HPP
#define VODK_DATA_RESOURCEMANAGER_HPP

#include "kiwi/Node.hpp"
#include "vodk/core/FreelistVector.hpp"
#include "vodk/core/Blob.hpp"

#include <vector>
#include <map>

namespace vodk {
namespace data {

typedef uint16_t ResourceNodeID;
typedef uint16_t ResourceOffset;

class ResourceManager;

struct ResourceID {
    ResourceNodeID nodeID;
    ResourceOffset offset;
};

enum ResourceState {
    RESOURCE_NONE,
    RESOURCE_EMPTY,
    RESOURCE_LOADING,
    RESOURCE_LOADED,
    RESOURCE_ERROR
};

static const uint32_t RESOURCE_TYPE_UNKNOWN = 0;

template<typename T>
struct ResourceTrait {
    enum { Type = RESOURCE_TYPE_UNKNOWN };
};

class Resource {
public:
    virtual uint32_t type() const = 0;
    template<typename T> T* cast() {
        if (type() == ResourceTrait<T>::Type) {
            return static_cast<T*>(this);
        }
        return nullptr;
    }
};

class ResourceVector {
public:
    virtual ~ResourceVector() {}
    virtual Resource* get(ResourceOffset offset, kiwi::PortIndex port);
    virtual void add_offset(ResourceOffset offset) = 0;
};

class ResourceNode : public kiwi::Node {
public:
    virtual ResourceVector* create_vector() = 0;
    virtual void update(Range<Resource*> inputs, Range<Resource*> outputs) = 0;
};

class ResourceGraph : public kiwi::Graph {
public:

protected:
    std::vector<ResourceNode*> _nodes;
};

class ResourceManager {
public:
    ResourceManager(ResourceGraph* graph)
    : _graph(graph)
    {}

protected:
    ResourceGraph* _graph;
    std::map<ResourceNodeID, ResourceVector*> _data;
};

enum ResourceType {
    RESOURCE_URL,
    RESOURCE_TEXT,
    RESOURCE_JSON_TREE,
    RESOURCE_IMAGE_PNG,
    RESOURCE_SHADER,
    RESOURCE_SHADER_PROGRAM,
    RESOURCE_FILE,
    RESOURCE_BUFFER,
    RESOURCE_TEXURE,
    RESOURCE_PROPERTY_LIST,
    RESOURCE_INT,
    RESOURCE_FLOAT,
    RESOURCE_VEC3,
    RESOURCE_MAT4,
    RESOURCE_TRIGGER,
    RESOURCE_CUSTOM
};

enum ResourceNodeType {
    NODE_READ_FILE,
    NODE_WRITE_FILE,
    NODE_COMPILE_SHADER,
    NODE_LINK_SHADER,
    NODE_CUSTOM
};

} // data
} // vodk

#endif
