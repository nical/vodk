
#ifndef KIWI_GRAPH_HPP
#define KIWI_GRAPH_HPP

#include <vector>
#include <limits>
#include <string>
#include "vodk/core/Range.hpp"

namespace kiwi2 {

typedef int32_t NodeTypeID;
typedef int32_t DataTypeID;
typedef int16_t NodeID;
typedef int16_t LinkID;
typedef uint8_t PortID;

static const NodeID INVALID_NODE_ID = -1;
static const LinkID INVALID_LINK_ID = -1;

class Node;
class Link;

typedef Range<LinkID> ConnectionRange;
typedef std::vector<LinkID> ConnectionVector;

template<typename T> struct DataTypeTrait {
    enum { Type = 0 };
};


class NodeIterator {
public:
    explicit NodeIterator(Range<Node*> r, NodeID id = 0)
    : _range(r), _id(id) {}

    NodeID get() const { return _id; }

    NodeID operator*() const { return get(); }

    NodeIterator operator++() {
        if (!_range.empty()) {
            _range = _range.next();
        }
        // skip zeros
        while (!_range.empty() && _range[0] == nullptr) {
            _range = _range.next();
            ++_id;
        }
        return *this;
    }

    NodeIterator operator++(int) {
        NodeIterator n = *this;
        ++(*this);
        return n;
    }

    bool operator==(const NodeIterator& rhs) {
        return _range == rhs._range;
    }

    bool operator!=(const NodeIterator& rhs) {
        return !(_range == rhs._range);
    }

protected:
    Range<Node*> _range;
    NodeID _id;
    friend bool operator==(const NodeIterator& lhs, const NodeIterator& rhs);
};

class NodeRange {
public:
    NodeRange(Range<Node*> range) : _range(range) {}

    NodeIterator begin() { return NodeIterator(_range); }

    NodeIterator end() {
        return NodeIterator(Range<Node*>(_range.pointer() + _range.size(),
                                         _range.pointer() + _range.size()));
    }
protected:
    Range<Node*> _range;
};

class TypeSystem;

class Graph {
public:

    Graph(TypeSystem* ts = nullptr, NodeTypeID self_type = 0);

    virtual ~Graph();

    NodeID add_node(NodeTypeID type);

    NodeID internal_node() { return 0; }

    void remove_node(NodeID n);

    bool are_connected(NodeID n1, NodeID n2);

    NodeTypeID get_node_type(NodeID n);

    NodeTypeID get_self_type();

    LinkID connect(NodeID n1, PortID p1, NodeID n2, PortID p2);

    void disconnect(LinkID l);

    void disconnect_inputs(NodeID n, PortID p);

    void disconnect_outputs(NodeID n, PortID p);

    void disconnect_all_inputs(NodeID n);

    void disconnect_all_outputs(NodeID n);

    ConnectionRange input_connections(NodeID n);

    ConnectionRange output_connections(NodeID n);

    ConnectionRange output_connections(NodeID n, PortID output_port);

    ConnectionRange input_connections(NodeID n, PortID input_port);

    NodeID node_before(LinkID l);

    NodeID node_after(LinkID l);

    NodeRange nodes();

    uint16_t num_nodes() { return _num_nodes; }

    uint16_t num_connections();

protected:
    Link* link(LinkID id) { return _connections[id]; }

    std::vector<Node*> _nodes;
    std::vector<Link*> _connections;
    TypeSystem* 	   _type_system;
    uint16_t 		   _num_nodes;
};

class TypeSystem {
public:
	virtual bool can_connect(Graph& g, NodeID n1, PortID p1, NodeID n2, PortID p2) { return true; }
    virtual bool contains_type(NodeTypeID type) = 0;
};

struct SimplePortDescriptor {
    DataTypeID type;
    std::string name;
};

struct SimpleNodeDescriptor {
public:
    std::string name;
    std::vector<SimplePortDescriptor> inputs;
    std::vector<SimplePortDescriptor> outputs;
};

class SimpleTypeSystem : public TypeSystem {
public:
    bool can_connect(Graph& g,
                     NodeID n1, PortID p1,
                     NodeID n2, PortID p2)
    {
        const SimpleNodeDescriptor& desc1 = _nodes[g.get_node_type(n1)];
        const SimpleNodeDescriptor& desc2 = _nodes[g.get_node_type(n2)];
        if (desc1.outputs.size() <= p1) return false;
        if (desc2.inputs.size() <= p2) return false;
        if (desc1.outputs[p1].type != desc2.inputs[p1].type) return false;
        return true;
    }

    NodeTypeID register_type(SimpleNodeDescriptor desc) {
        _nodes.push_back(desc);
        return _nodes.size() - 1;
    }

    bool contains_type(NodeTypeID type) {
        return (type < _nodes.size());
    }

    std::vector<SimpleNodeDescriptor> _nodes;
};

} // kiwi

#endif
