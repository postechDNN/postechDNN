#ifndef __POLYGON_H__
#define __POLYGON_H__

#include "Point.h"
#include "Edge.h"
#include <iostream>
#include <vector>

//Simple Polygon without holes =>Ring
class SimplePolygon {
private:
	int n_vertices;
	std::vector<Point> vertices; //clockwise order
	std::vector<Edge> edges;
public:

	SimplePolygon(std::vector<Point>& v, bool parallel = true);
	SimplePolygon(std::vector<Edge>& e);
	~SimplePolygon();
	int inPolygon(Point p);
	std::vector<Point> getVertices();
	std::vector<Edge> getEdges();
};

#endif