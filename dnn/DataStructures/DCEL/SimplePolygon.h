#pragma once
#include <iostream>
#include <vector>
#include <iostream>
//#include "./..//..//Algorithms/ESP_plane/Edge.h"
#include "DCEL.h"

class HEdge;

class SimplePolygon {
public:
	std::vector<HEdge*>* edges; //The edges should be stored in clock-wise order.
	// bool checkSimple(std::vector<Edge*>*);
public:
	SimplePolygon();
	SimplePolygon(std::vector<HEdge*>*);
	int inPolygon(Point p);
	std::vector<HEdge*>* getHEdges();
};