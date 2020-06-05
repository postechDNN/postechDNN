#pragma once
#include <vector>
#include <iostream>
#include "Edge.h"

class SimplePolygon {
private:
	std::vector<Edge*>* edges; //The edges should be stored in clock-wise order.
	bool checkSimple(std::vector<Edge*>*);
public:
	SimplePolygon();
	SimplePolygon(std::vector<Edge*>*);
	std::vector<Edge*>* getEdges();
};