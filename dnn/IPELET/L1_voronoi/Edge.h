#ifndef __EDGE_H__
#define __EDGE_H__

#include <iostream>
#include "Point.h"
#include <cmath>

class Edge {
protected:
	Point s, t;
public:
	Edge();
	Edge(Point s, Point t);
	~Edge();
	Point getOrigin() const;
	Point getDest() const;
	Point getDir();

	double turn_direction(Point op);
	bool crossing(Edge& op,Point *P = nullptr, bool closed = true);
	bool on(Point p);
	bool parallel(Edge &op, bool dir = false);
	Edge Union(Edge &op);
	bool operator <(Edge& operand);
	bool operator ==(Edge& operand);
	friend std::ostream& operator<< (std::ostream& os, const Edge& op);
};

#endif