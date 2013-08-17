
#ifndef KIWI_NODE_HPP
#define KIWI_NODE_HPP

#include <stdint.h>
#include <vector>
#include "vodk/core/Range.hpp"

namespace kiwi {

typedef int8_t PortIndex;
typedef uint32_t NodeID; /// 0 should always be considered a blank/invalid ID
typedef uint32_t NodeTypeID; /// 0 should always be considered a blank/invalid ID
typedef uint32_t PortTypeID; /// 0 should always be considered a blank/invalid ID
class Node;
class Graph;

enum Direction {
    INPUT,
    OUTPUT,
};

class TypeSystem {

};

struct Link {
    Link(Node* outNode, Node* inNode, PortIndex outPort, PortIndex inPort)
    : outputNode(outNode), inputNode(inNode)
    , outputPort(outPort), inputPort(inPort)
    {}

    Node* outputNode;     // -->
    Node* inputNode;      // -->
    PortIndex outputPort; // <--
    PortIndex inputPort;  // <--
};

typedef Range<Link*> LinkRange;

class FilterLinkRange {
public:
    FilterLinkRange(Link** start, Link** end, PortIndex inFilter, PortIndex outFilter)
    : _start(start), _end(end), _inIndex(inFilter), _outIndex(outFilter)
    {}

    FilterLinkRange next();

    bool empty() { return _start >= _end; }

    const Link* get_link() { return *_start; }

    const Link& operator*() { return **_start; }

    const Link* operator->() { return *_start; }

protected:
    Link** _start;
    Link** _end;
    PortIndex _inIndex;
    PortIndex _outIndex;
};

class FilteredInputConnectionRange {
public:
    FilteredInputConnectionRange(LinkRange range, PortIndex input)
    : _range(range), _input(input) {}

    bool empty() { return _range.empty(); }

    const Link* get() { return *_range.pointer(); }

    const Link& operator*() { return *get(); }

    const Link* operator->() { return get(); }

    FilteredInputConnectionRange next() {
        auto r = *this;
        while (!r.empty()) {
            if (r.get()->inputPort == _input) {
                break;
            }
        }
        return r;
    }

protected:
    LinkRange _range;
    PortIndex _input;
};

template<typename T>
class PreviousNodeRange {
public:

    PreviousNodeRange(T range)
    : _range(range) {}

    bool empty() { return _range.empty(); }

    const Link* get() { return _range->outputNode(); }

    const Link& operator*() { return *get(); }

    const Link* operator->() { return get(); }
protected:
    T _range;
};

template<typename T>
class NextNodeRange {
public:

    NextNodeRange(T range)
    : _range(range) {}

    bool empty() { return _range.empty(); }

    const Link* get() { return _range->inputNode(); }

    const Link& operator*() { return *get(); }

    const Link* operator->() { return get(); }
protected:
    T _range;
};

class ConnectedNodeIterator : public FilterLinkRange
{
public:
    const Node* get_node() {
        switch (_direction) {
            case INPUT: return get_link() ? get_link()->inputNode : nullptr;
            case OUTPUT: return get_link() ? get_link()->outputNode : nullptr;
        }
        return nullptr;
    }

    const Node& operator*() { return *get_node(); }
 
    const Node* operator->() { return get_node(); }

protected:
    kiwi::Direction _direction;
};

class Node {
public:
    static const int NO_FILTER = -1;

    Node(Graph* graph, NodeTypeID type = 0)
    : _graph(graph), _typeID(type) {}

    LinkRange input_connections() {
        return LinkRange(_inputs.data(), _inputs.size());
    }

    LinkRange output_connections() {
        return LinkRange(_outputs.data(), _outputs.size());
    }

    FilterLinkRange input_connections(PortIndex filter);

    FilterLinkRange output_connections(PortIndex filter);

    ConnectedNodeIterator previous_nodes(PortIndex filter);

    ConnectedNodeIterator next_nodes(PortIndex filter);

    NodeTypeID get_typeid() const { return _typeID; }

    Graph* get_graph() { return _graph; }

    std::vector<Link*> _inputs;
    std::vector<Link*> _outputs;
    Graph*             _graph;
    NodeTypeID         _typeID;
};

struct PortDescriptor {
    std::vector<PortTypeID> inputs;
    std::vector<PortTypeID> outputs;
};

class Graph : public Node {
public:
    Graph() : Node(nullptr) {}

    virtual bool connect(Node& n1, PortIndex p1, Node& n2, PortIndex p2);

    virtual bool disconnect(const Link* l);

    virtual bool are_connected(Node& n1, PortIndex p1, Node& n2, PortIndex p2);
};

class BasicGraph : public Graph {
public:
    typedef Node** Iterator;

    Node* create_node(NodeTypeID node_type);

    void delete_node(Node* n);

    Range<Node*> nodes() { return Range<Node*>(_nodes.data(), _nodes.size()); }

protected:
    std::vector<Node*> _nodes;
    Node* _internal_node;
};


namespace base {

bool connect(Node& n1, PortIndex p1, Node& n2, PortIndex p2);

bool disconnect(const Link* l);

bool are_connected(Node& n1, PortIndex p1, Node& n2, PortIndex p2);

} // base

} // kiwi

#endif
