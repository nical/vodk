
#include "kiwi/Graph.hpp"

namespace kiwi2 {

struct Link {
    Link(NodeID n1, PortID p1, NodeID n2, PortID p2)
    : node_1(n1), node_2(n2), port_1(p1), port_2(p2) {}

    NodeID node_1; // -->
    NodeID node_2; // <--
    PortID port_1; // -->
    PortID port_2; // <--
};

typedef Range<LinkID> ConnectionRange;
typedef std::vector<LinkID> ConnectionVector;

struct Node {
    Node(NodeTypeID id = 0) : type_id(id) {}
    ConnectionVector input_connections;
    ConnectionVector output_connections;
    NodeTypeID type_id;
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

    Graph() : _num_nodes(1) {
        _nodes.push_back(new Node(0));
    }

    virtual ~Graph() {
        for (auto& n : _nodes) {
            delete n;
        }
        for (auto& l : _connections) {
            delete l;
        }
    }

    NodeID add_node(NodeTypeID type) {
        NodeID result = INVALID_NODE_ID;
        ++ _num_nodes;
        for (NodeID i = 0; i < _nodes.size(); ++i) {
            if (!_nodes[i]) {
                _nodes[i] = new Node(type);
                return i;
            }
        }
        _nodes.push_back(new Node(type));
        return _nodes.size()-1;
    }

    NodeID internal_node() {
        return 0;
    }

    void remove_node(NodeID n) {
        assert(_nodes[n]);
        assert(n != 0);
        --_num_nodes;
        // TODO
    }

    bool are_connected(NodeID n1, NodeID n2) {
        return false; // TODO
    }

    NodeTypeID get_node_type(NodeID n) {
        assert(n < _nodes.size());
        assert(_nodes[n]);
        return _nodes[n]->type_id;
    }

    LinkID connect(NodeID n1, PortID p1, NodeID n2, PortID p2) {
        // connections are stored in order of increasing port
        auto it1 = _nodes[n1]->output_connections.begin();
        auto stop1 = _nodes[n1]->output_connections.end();
        while (it1 != stop1 && link(*it1)->port_1 < p1) { ++it1; }

        auto it2 = _nodes[n2]->input_connections.begin();
        auto stop2 = _nodes[n2]->input_connections.end();
        while (it2 != stop2 && link(*it2)->port_2 < p2) { ++it2; }

        Link* l = new Link(n1,p1,n2,p2);

        int result = -1;
        for (int i = 0; i < _connections.size(); ++i) {
            if (!_connections[i]) {
                _connections[i] = l;
                result = i;
                break;
            }
        }
        if (result < 0) {
            _connections.push_back(l);
            result = _connections.size() - 1;     
        }
        _nodes[n1]->output_connections.insert(it1, result);
        _nodes[n2]->input_connections.insert(it2, result);
        return result;
    }

    void disconnect(LinkID l) {
        Node& n1 = *_nodes[link(l)->node_1];
        Node& n2 = *_nodes[link(l)->node_2];

        auto it1 = n1.output_connections.begin();
        auto stop1 = n1.output_connections.end();
        while (it1 != stop1) {
            if (*it1 == l) {
                n1.output_connections.erase(it1);
            }
            ++it1;
        }
        auto it2 = n2.input_connections.begin();
        auto stop2 = n2.input_connections.end();
        while (it2 != stop2) {
            if (*it2 == l) {
                n2.input_connections.erase(it2);
            }
            ++it2;
        }
        delete _connections[l];
        _connections[l] = nullptr;
    }

    void disconnect_inputs(NodeID n, PortID p) {
        while (input_connections(n, p).size() > 0) {
            disconnect(input_connections(n, p)[0]);
        }
    }

    void disconnect_outputs(NodeID n, PortID p) {
        while (output_connections(n, p).size() > 0) {
            disconnect(output_connections(n, p)[0]);
        }
    }

    void disconnect_all_inputs(NodeID n) {
        while (input_connections(n).size() > 0) {
            disconnect(input_connections(n)[0]);
        }
    }

    void disconnect_all_outputs(NodeID n) {
        while (output_connections(n).size() > 0) {
            disconnect(output_connections(n)[0]);
        }
    }

    ConnectionRange input_connections(NodeID n) {
        ConnectionVector& cv = _nodes[n]->input_connections;
        return ConnectionRange(cv.data(), cv.size());
    }

    ConnectionRange output_connections(NodeID n) {
        ConnectionVector& cv = _nodes[n]->output_connections;
        return ConnectionRange(cv.data(), cv.size());
    }

    ConnectionRange output_connections(NodeID n, PortID output_port) {
        ConnectionVector& connections = _nodes[n]->output_connections;
        for(int i = 0; i < connections.size(); ++i) {
            if (link(connections[i])->port_1 == output_port) {
                int j = i;
                while (j < connections.size()
                       && link(connections[j])->port_1 == output_port) {
                    ++j;
                }
                return ConnectionRange(&connections[i], j-i);
            }
        }
        return ConnectionRange();
    }

    ConnectionRange input_connections(NodeID n, PortID input_port) {
        ConnectionVector& connections = _nodes[n]->input_connections;
        for(int i = 0; i < connections.size(); ++i) {
            if (link(connections[i])->port_2 == input_port) {
                int j = i;
                while (j < connections.size()
                       && link(connections[j])->port_2 == input_port) {
                    ++j;
                }
                return ConnectionRange(&connections[i], j-i);
            }
        }
        return ConnectionRange();
    }

    NodeID node_before(LinkID l) {
        assert(l < _connections.size());
        return _connections[l]->node_1;
    }

    NodeID node_after(LinkID l) {
        assert(l < _connections.size());
        return _connections[l]->node_2;
    }

    NodeRange nodes() {
        return NodeRange(Range<Node*>(_nodes.data(), _nodes.size()));
    }

    uint16_t num_nodes() { return _num_nodes; }

    uint16_t num_connections() {
        uint16_t count = 0;
        for (auto c : _connections) {
            if (c != nullptr) { ++count; }
        }
        return count;
    }

protected:
    Link* link(LinkID id) { return _connections[id]; }

    std::vector<Node*> _nodes;
    std::vector<Link*> _connections;
    uint16_t _num_nodes;
};

namespace unittest {

void graph_test() {
    Graph graph;
    auto n1 = graph.add_node(1);
    auto n2 = graph.add_node(2);
    auto n3 = graph.add_node(3);

    assert(graph.num_connections() == 0);

    assert(graph.get_node_type(n1) == 1);
    assert(graph.get_node_type(n2) == 2);
    assert(graph.get_node_type(n3) == 3);

    for (int i = 0; i < 10; ++i) {
        assert(graph.output_connections(n1, i).size() == 0);
        assert(graph.input_connections(n2, i).size() == 0);
    }
    assert(graph.output_connections(n1).size() == 0);
    assert(graph.input_connections(n2).size() == 0);
    auto l1 = graph.connect(n1, 0, n2, 0);
    assert(graph.num_connections() == 1);
    assert(graph.output_connections(n1).size() == 1);
    assert(graph.input_connections(n2).size() == 1);
    assert(graph.output_connections(n1, 0).size() == 1);
    assert(graph.output_connections(n1, 0)[0] == l1);
    assert(graph.input_connections(n2, 0)[0] == l1);
    for (int i = 1; i < 10; ++i) {
        assert(graph.output_connections(n1, i).size() == 0);
        assert(graph.input_connections(n2, i).size() == 0);
    }
    assert(graph.node_before(l1) == n1);
    assert(graph.node_after(l1) == n2);

    graph.disconnect(l1);
    assert(graph.num_connections() == 0);

    for (int i = 0; i < 10; ++i) {
        assert(graph.output_connections(n1, i).size() == 0);
        assert(graph.input_connections(n2, i).size() == 0);
    }

    auto l2 = graph.connect(n1, 0, n2, 0);
    auto l3 = graph.connect(n1, 2, n2, 2);
    auto l4 = graph.connect(n1, 1, n2, 1);
    assert(graph.num_connections() == 3);
    assert(graph.output_connections(n1).size() == 3);
    assert(graph.input_connections(n2).size() == 3);
    for (int i = 0; i < 3; ++i) {
        assert(graph.output_connections(n1, i).size() == 1);
        assert(graph.input_connections(n2, i).size() == 1);
    }
    graph.disconnect(l3);
    assert(graph.num_connections() == 2);
    assert(graph.output_connections(n1, 0).size() == 1);
    assert(graph.input_connections(n2, 0).size() == 1);
    assert(graph.output_connections(n1, 1).size() == 1);
    assert(graph.input_connections(n2, 1).size() == 1);
    assert(graph.output_connections(n1, 2).size() == 0);
    assert(graph.input_connections(n2, 2).size() == 0);
    graph.disconnect_inputs(n2, 0);
    graph.disconnect_inputs(n2, 1);
    graph.disconnect_inputs(n2, 2);
    assert(graph.num_connections() == 0);
    for (int i = 0; i < 10; ++i) {
        assert(graph.output_connections(n1, i).size() == 0);
        assert(graph.input_connections(n2, i).size() == 0);
    }

    auto l5 = graph.connect(n1, 3, n2, 1);
    assert(graph.num_connections() == 1);

    auto l6 = graph.connect(n1, 1, n2, 1);
    assert(graph.num_connections() == 2);

    auto l7 = graph.connect(n1, 1, n2, 3);
    assert(graph.num_connections() == 3);

    assert(graph.output_connections(n1).size() == 3);
    assert(graph.output_connections(n1, 3).size() == 1);
    assert(graph.output_connections(n1, 1).size() == 2);
    assert(graph.input_connections(n2).size() == 3);
    assert(graph.input_connections(n2, 3).size() == 1);
    assert(graph.input_connections(n2, 1).size() == 2);

    graph.disconnect_outputs(n1, 3);
    assert(graph.output_connections(n1).size() == 2);
    assert(graph.num_connections() == 2);

    graph.disconnect_outputs(n1, 1);
    assert(graph.output_connections(n1).size() == 0);
    assert(graph.num_connections() == 0);

    graph.disconnect_outputs(n1, 1);
    graph.disconnect_outputs(n1, 2);

    assert(graph.output_connections(n1).size() == 0);
    assert(graph.input_connections(n2).size() == 0);

    int i = 0;
    for (auto n : graph.nodes()) {
        ++i;
    }
    assert (i == graph.num_nodes());
}

} // unittest

} // kiwi