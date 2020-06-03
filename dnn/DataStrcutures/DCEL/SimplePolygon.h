#pragma once
#include <vector>
#include <iostream>
#include "Edge.h"

class SimplePolygon {
private:
	std::vector<Edge*>* edges;
	bool checkSimple(std::vector<Edge*>*);
public:
	SimplePolygon();
	SimplePolygon(std::vector<Edge*>*);
	std::vector<Edge*>* getEdges();
};