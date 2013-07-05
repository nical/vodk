
#ifndef KIWI_NODE_HPP
#define KIWI_NODE_HPP

#include <stdint.h>
#include <vector>

/*
TODO:
    * generic way to iterate over a kiwi::Graph
    * basic Kiwi::TypeSystem
*/

namespace kiwi {

typedef int8_t PortIndex;
class Node;
class Graph;

enum Direction {
    INPUT,
    OUTPUT,
};

struct Link {
    Node* outputNode;     // -->
    Node* inputNode;      // -->
    PortIndex outputPort; // <--
    PortIndex inputPort;  // <--

    Link(Node* outNode, Node* inNode, PortIndex outPort, PortIndex inPort)
    : outputNode(outNode), inputNode(inNode)
    , outputPort(outPort), inputPort(inPort)
    {}
};

class LinkIterator {
public:
    LinkIterator(Link** start, Link** end, PortIndex inFilter, PortIndex outFilter)
    : _start(start), _end(end), _inIndex(inFilter), _outIndex(outFilter)
    {}
    bool empty() {
        return _start >= _end;
    }
    LinkIterator next() {
        Link** i = _start;
        while (i != _end) {
            ++i;
            if ((_inIndex < 0 || _inIndex == (*i)->inputPort) &&
                (_outIndex < 0 || _outIndex == (*i)->outputPort)) {
                break;
            }
        }
        return LinkIterator(i, _end, _inIndex, _outIndex);
    }
    const Link* getLink() {
        return *_start;
    }
    const Link& operator*() {
        return **_start;
    }
    const Link* operator->() {
        return *_start;
    }
protected:
    Link** _start;
    Link** _end;
    PortIndex _inIndex;
    PortIndex _outIndex;
};

class ConnectedNodeIterator : public LinkIterator
{
public:
    const Node* getNode() {
        switch (_direction) {
            case INPUT: return getLink() ? getLink()->inputNode : nullptr;
            case OUTPUT: return getLink() ? getLink()->outputNode : nullptr;
        }
        return nullptr;
    }
    const Node& operator*() {
        return *getNode();
    }
    const Node* operator->() {
        return getNode();
    }
protected:
    kiwi::Direction _direction;
};

typedef uint32_t NodeTypeID; /// 0 should always be considered a blank/invalid ID
typedef uint32_t PortTypeID; /// 0 should always be considered a blank/invalid ID

class Node {
public:
    friend class kiwi::Graph;

    Node(Graph* graph, NodeTypeID type = 0)
    : _graph(graph), _typeID(type) {}
    virtual ~Node() {}

    static const int NO_FILTER = -1;

    LinkIterator inputConnections(PortIndex filter = NO_FILTER) {
        if (_inputs.size() == 0) return LinkIterator(nullptr, nullptr, NO_FILTER, NO_FILTER);
        return LinkIterator(&_inputs[0], &_inputs[_inputs.size()-1], filter, NO_FILTER);
    }
    LinkIterator outputConnections(PortIndex filter = NO_FILTER) {
        if (_outputs.size() == 0) return LinkIterator(nullptr, nullptr, NO_FILTER, NO_FILTER);
        return LinkIterator(&_outputs[0], &_outputs[_outputs.size()-1], NO_FILTER, filter);
    }

    PortIndex getInputCount() { return _inputs.size(); }
    PortIndex getOutputCount() { return _outputs.size(); }
    virtual PortTypeID getInputTypeID(PortIndex) { return 0; }
    virtual PortTypeID getOutputTypeID(PortIndex) { return 0; }

    NodeTypeID getTypeID() const { return _typeID; }

    Graph* getGraph() { return _graph; }

private:
    std::vector<Link*> _inputs;
    std::vector<Link*> _outputs;
    Graph*             _graph;
    NodeTypeID         _typeID;
};

class INodeIterator {
public:
    virtual ~INodeIterator() {}
    virtual Node& get() = 0;
    virtual bool next() = 0;
};

class Graph {
public:
    virtual bool connect(Node& n1, PortIndex p1,
                        Node& n2, PortIndex p2);
    virtual bool areConnected(Node& n1, PortIndex p1,
                             Node& n2, PortIndex p2);
    virtual bool disconnect(const Link* l);

    //virtual INodeIterator* nodes() = 0;
};

} // namespace

#endif
