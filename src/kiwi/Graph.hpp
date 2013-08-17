
#ifndef KIWI_GRAPH_HPP
#define KIWI_GRAPH_HPP

#include <vector>
#include <limits>
#include "vodk/core/Range.hpp"

namespace kiwi2 {

typedef uint32_t NodeTypeID;
typedef uint16_t NodeID;
typedef uint16_t LinkID;
typedef uint8_t PortID;

class Node;
class Link;

typedef Range<LinkID> ConnectionRange;
typedef std::vector<LinkID> ConnectionVector;

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

protected:
    Range<Node*> _range;
    NodeID _id;
    friend bool operator==(const NodeIterator& lhs, const NodeIterator& rhs);
};

bool operator==(const NodeIterator& lhs, const NodeIterator& rhs) {
    return lhs._range == rhs._range;
}

bool operator!=(const NodeIterator& lhs, const NodeIterator& rhs) {
    return !(lhs == rhs);
}


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


class Graph {
public:
    static const NodeID INVALID_NODE_ID = std::numeric_limits<NodeID>::max();

    Graph();

    virtual ~Graph();

    NodeID add_node(NodeTypeID type);

    NodeID internal_node() { return 0; }

    void remove_node(NodeID n);

    bool are_connected(NodeID n1, NodeID n2);

    NodeTypeID get_node_type(NodeID n);

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
    uint16_t _num_nodes;
};


} // kiwi

#endif
