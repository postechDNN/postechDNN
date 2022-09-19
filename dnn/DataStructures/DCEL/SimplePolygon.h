#pragma once
#include <iostream>
#include <vector>
#include <iostream>
#include "./..//..//Algorithms/ESP_plane/Edge.h"

class SimplePolygon {
private:
	std::vector<Edge*>* edges; //The edges should be stored in clock-wise order.
	bool checkSimple(std::vector<Edge*>*);
public:
	SimplePolygon();
	SimplePolygon(std::vector<Edge*>*);
	int inPolygon(Point p);
	std::vector<Edge*>* getEdges();
};