#ifndef __EDGE_H__
#define __EDGE_H__

#include <iostream>
#include "Point.h"

class Edge {
protected:
	Point s, t;
	int type; // 0 = segment, 1 = parabola
	Point focus;
	Point dOrigin;
	Point dRight;

public:
	Edge();
	Edge(Point s, Point t);
	Edge(Point s, Point t, Point f, Point dx, Point dy);
	~Edge();
	Point getOrigin() const;
	Point getDest() const;
	Point getDir();

	double turn_direction(Point op);
	bool intersect(Edge& op, double* i_x = NULL, double* i_y = NULL);
	bool operator <(Edge& operand);
	bool operator ==(Edge& operand);
};

#endif