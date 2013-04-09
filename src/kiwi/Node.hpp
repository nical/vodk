
#ifndef KIWI_NODE_HPP
#define KIWI_NODE_HPP

#include <stdint.h>
#include <vector>

namespace kiwi {

typedef int8_t PortIndex;
class Node;

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
    const Link& operator*() {
        return **_start;
    }
    const Link* operator->() {
        return *_start;
    }
private:
    Link** _start;
    Link** _end;
    PortIndex _inIndex;
    PortIndex _outIndex;
};

class Node {
public:
    static const int NO_FILTER = -1;
    static bool connect(Node& n1, PortIndex p1,
                        Node& n2, PortIndex p2);
    static bool areConnected(Node& n1, PortIndex p1,
                             Node& n2, PortIndex p2);
    static bool disconnect(const Link* l);

    LinkIterator inputConnections(PortIndex filter = NO_FILTER) {
        if (_inputs.size() == 0) return LinkIterator(nullptr, nullptr, NO_FILTER, NO_FILTER);
        return LinkIterator(&_inputs[0], &_inputs[_inputs.size()-1], filter, NO_FILTER);
    }
    LinkIterator outputConnections(PortIndex filter = NO_FILTER) {
        if (_outputs.size() == 0) return LinkIterator(nullptr, nullptr, NO_FILTER, NO_FILTER);
        return LinkIterator(&_outputs[0], &_outputs[_outputs.size()-1], NO_FILTER, filter);
    }
private:
    std::vector<Link*> _inputs;
    std::vector<Link*> _outputs;
};


} // namespace


#endif
