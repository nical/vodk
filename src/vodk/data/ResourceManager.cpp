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

#include "vodk/data/ResourceManager.hpp"
#include "vodk/core/Blob.hpp"
#include <vector>

namespace vodk {
namespace data {

class IntResource : public Resource {
public:
    virtual uint32_t type() const override { return 1; }
    int32_t value;
};

template<> struct ResourceTrait<IntResource> { enum { Type = 1 }; };

template<uint32_t N>
class IntResourceVector : public ResourceVector {
public:

    virtual Resource* get(ResourceOffset offset, kiwi::PortIndex port) {
        return &_data[offset][port];
    }

    virtual void add_offset(ResourceOffset offset) override {
        if (offset >= _data.size()) {
            _data.resize(offset+1);
        }
    }
private:
    std::vector<Array<IntResource,N> > _data;
};

class AddNode : public ResourceNode {
public:
    virtual ResourceVector* create_vector() override {
        return new IntResourceVector<1>();
    }
    virtual void update(Range<Resource*> inputs, Range<Resource*> outputs) {
        outputs[0]->cast<IntResource>()->value
            = inputs[0]->cast<IntResource>()->value
            + inputs[1]->cast<IntResource>()->value;
    }
};

namespace unittest {
    void ResourceManager() {
        //ResourceManager mgr;
    }
}

} // data
} // vodk
