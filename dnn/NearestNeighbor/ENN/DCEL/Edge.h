#pragma once
#include "Point.h"

class Edge {
protected:
	Point s,  t;
public:
	Edge();
	Edge(const Point& _s, const Point& _t);
	Edge(const Edge&);
	~Edge();
	bool operator==(Edge&);
	bool on(Point& p);
	Point* crossing(Edge&, bool);
	Point gets();
	Point gett();
	void sets(Point&);
	void sett(Point&);
	double length();
};
