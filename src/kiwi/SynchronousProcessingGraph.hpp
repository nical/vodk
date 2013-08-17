
#ifndef KIWI_SYNCHRONOUSPROCESSINGGRAPH_HPP
#define KIWI_SYNCHRONOUSPROCESSINGGRAPH_HPP

#include "kiwi/Node.hpp"
#include "vodk/core/FreelistVector.hpp"

#include <vector>
#include <map>
#include <tuple>


namespace kiwi {

typedef uint16_t DataOffset;
typedef uint32_t DataTypeIndex;

template<typename T> struct DataTypeTrait {
    enum { Type = 0 };
};


class DataVector {
public:

    virtual void* get_data(DataOffset i) = 0;
    virtual void add_data(DataOffset i) = 0;
    virtual void remove_data(DataOffset i) = 0;
    virtual DataTypeIndex type() { return 0; }

    template<typename T> T* get(DataOffset i) {
        assert(DataTypeTrait<T>::Type != 0
               && DataTypeTrait<T>::Type == type());
        return static_cast<T*>(get_data(i));
    }
};

template<typename T> class TDataVector : public DataVector {
public:
    virtual void* get_data(DataOffset i) override {
        return &_data[i];
    }

    virtual void add_data(DataOffset i) override {
        _data.add(T());
    }

    virtual void remove_data(DataOffset i) override {
        _data.remove(i);
    }

    virtual DataTypeIndex type() override { return DataTypeTrait<T>::Type; }

    vodk::FreelistVector<T> _data;
};

class NodeData {
public:
    NodeData(NodeID nid, NodeTypeID ntid)
    : _node_id(nid), _node_type_id(ntid)
    {}

    void* get_data(DataOffset i, PortIndex p = 0) {
        return _data[p]->get_data(i);
    }

    template<typename T> T* get(DataOffset i, PortIndex p = 0) {
        return _data[p]->get<T>(i);
    }

    void add_data(DataOffset i) {
        for (auto d : _data) {
            d->add_data(i);
        }
    }

    void remove_data(DataOffset i) {
        for (auto d : _data) {
            d->remove_data(i);
        }
    }

    void add_vector(DataVector* vec) {
        _data.push_back(vec);
    }

protected:
    std::vector<DataVector*> _data;
    NodeID _node_id;
    NodeTypeID _node_type_id;
};

struct NodeDescriptor {
    PortIndex num_inputs;
    PortIndex num_outputs;
};

class BasicChainProcessor {
public:
    virtual DataVector* create_data_vector(NodeTypeID node_type) = 0;
    virtual void update(NodeTypeID node_type, Range<void*> inputs, Range<void*> outputs) = 0;
    virtual const NodeDescriptor& get_descriptor(NodeTypeID node_type) = 0;
};

class BasicChainProcessingContext {
public:
    void update(NodeID node, DataOffset offset) {

    }

protected:
    Graph* _graph;
    BasicChainProcessor* processor;
    std::map<NodeID, NodeData*> _node_data;
};

} // kiwi

#endif
