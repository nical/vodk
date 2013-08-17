
#include "Node.hpp"
#include <assert.h>
#include <kiwi/SynchronousProcessingGraph.hpp>

namespace kiwi {

bool base::connect(Node& n1, PortIndex p1,
                   Node& n2, PortIndex p2)
{
    if (are_connected(n1,p1,n2,p2)) {
        return true;
    }
    Link* l = new Link(&n1, &n2, p1, p2);
    n1._outputs.push_back(l);
    n2._inputs.push_back(l);
    return true;
}

bool base::are_connected(Node& n1, PortIndex p1,
                         Node& n2, PortIndex p2)
{
    auto it = n1.output_connections(p1);
    while (!it.empty()) {
        if (it->inputNode == &n2 && it->inputPort == p2) {
            return true;
        }
        it = it.next();
    }
    return false;
}

bool base::disconnect(const Link* l)
{
    Link* link = const_cast<Link*>(l); 

    std::vector<Link*>::iterator it1 = link->outputNode->_outputs.begin();
    std::vector<Link*>::iterator stop1 = link->outputNode->_outputs.end();
    while (it1 != stop1) {
        if (*it1 == link) {
            link->outputNode->_outputs.erase(it1);
            break;
        }
        ++it1;
    }
    std::vector<Link*>::iterator it2 = link->inputNode->_inputs.begin();
    std::vector<Link*>::iterator stop2 = link->inputNode->_inputs.end();
    while (it2 != stop2) {
        if (*it2 == link) {
            link->inputNode->_inputs.erase(it2);
            break;
        }
        ++it2;
    }

    assert(it1 != stop1 && it2 != stop2);
    delete l;
    return true;
}

FilterLinkRange Node::input_connections(PortIndex filter) {
    if (_inputs.size() == 0) return FilterLinkRange(nullptr, nullptr, NO_FILTER, NO_FILTER);
    return FilterLinkRange(&_inputs[0], &_inputs[_inputs.size()-1], filter, NO_FILTER);
}
FilterLinkRange Node::output_connections(PortIndex filter) {
    if (_outputs.size() == 0) return FilterLinkRange(nullptr, nullptr, NO_FILTER, NO_FILTER);
    return FilterLinkRange(&_outputs[0], &_outputs[_outputs.size()-1], NO_FILTER, filter);
}


FilterLinkRange FilterLinkRange::next() {
    Link** i = _start;
    while (i != _end) {
        ++i;
        if ((_inIndex < 0 || _inIndex == (*i)->inputPort) &&
            (_outIndex < 0 || _outIndex == (*i)->outputPort)) {
            break;
        }
    }
    return FilterLinkRange(i, _end, _inIndex, _outIndex);
}


bool Graph::connect(Node& n1, PortIndex p1, Node& n2, PortIndex p2) {
    if (n1.get_graph() && n1.get_graph() != this) {
        return false;
    }
    if (n2.get_graph() && n2.get_graph() != this) {
        return false;
    }
    base::connect(n1, p1, n2, p2);
}

bool Graph::disconnect(const Link* l) {
    base::disconnect(l);
}

bool Graph::are_connected(Node& n1, PortIndex p1,
                           Node& n2, PortIndex p2) {
    return base::are_connected(n1,p1,n2,p2);
}

Node* BasicGraph::create_node(NodeTypeID node_type) {
    _nodes.push_back(new Node(this, node_type));
    return _nodes[_nodes.size() - 1];
}

void BasicGraph::delete_node(Node* n) {
    auto it = _nodes.begin();
    while (*it != n) { ++it; }
    _nodes.erase(it);
    delete n;
}

void text_nodes() {
    BasicGraph graph;
}

} // namespace
