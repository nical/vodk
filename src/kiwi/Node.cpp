
#include "Node.hpp"
#include <assert.h>

namespace kiwi {

bool Node::connect(Node& n1, PortIndex p1,
                   Node& n2, PortIndex p2)
{
    if (areConnected(n1,p1,n2,p2)) {
        return true;
    }
    Link* l = new Link(&n1, &n2, p1, p2);
    n1._outputs.push_back(l);
    n2._inputs.push_back(l);
    return true;
}

bool Node::areConnected(Node& n1, PortIndex p1,
                        Node& n2, PortIndex p2)
{
    LinkIterator it = n1.outputConnections(p1);
    while (!it.empty()) {
        if (it->inputNode == &n2 && it->inputPort == p2) {
            return true;
        }
        it = it.next();
    }
    return false;
}

bool Node::disconnect(const Link* l) 
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

} // namespace
