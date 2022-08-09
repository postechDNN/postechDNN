#pragma once
#include "Point.h"

class Edge {
protected:
	Point* s, * t;
public:
	Edge(Point* _s, Point* _t);
	~Edge();
	bool crossing(Edge*);
	bool on(Point*);
	Point* gets();
	Point* gett();
};
