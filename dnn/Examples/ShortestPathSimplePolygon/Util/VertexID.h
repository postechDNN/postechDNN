#pragma once
#include "Point.h"
#include "Edge.h"
#include <vector>
using namespace std;

class VertexID {
private:
	int pointID;
	int polygonID;
	bool left_chain;
public:
	VertexID() {
		pointID = -1;
		polygonID = -1;
		left_chain = false;
	}
	VertexID(int _pointID, int _polygonID) {
		pointID = _pointID;
		polygonID = _polygonID;
		left_chain = false;
	}
	int get_pointID() {
		return pointID;
	}
	int get_polygonID() {
		return polygonID;
	}
	void set_left_chain(bool _left_chain) {
		left_chain = _left_chain;
		return;
	}
	bool get_left_chain() {
		return left_chain;
	}

};
