
#ifndef KIWI_GRAPH_HPP
#define KIWI_GRAPH_HPP

#include <vector>

namespace kiwi2 {

typedef uint16_t NodeID;
typedef uint8_t PortID;

enum ConnectionState {
	KIWI_DISCONNECTED,
	KIWI_CONNECTED,
}

struct Link {
	NodeID node;
	PortID port;
	ConnectionState state;
};

struct Node {
	std::vector<Connection> _input_connections
	std::vector<Connection> _output_connections
};

class Graph {

};



} // kiwi

#endif
