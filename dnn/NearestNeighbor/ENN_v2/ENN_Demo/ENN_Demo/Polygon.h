#pragma once

#include "Point.h"
#include "Edge.h"
#include "pch.h"
#include <iostream>
#include <vector>

//Simple Polygon without holes =>Ring
class SimplePolygon {
private:
	int n_vertices;
	std::vector<Point> vertices; //it is stored as clockwise order
	std::vector<Edge> edges;
public:

	SimplePolygon(std::vector<Point>& v, bool colinear = true);  //allow colinear edges
	SimplePolygon(std::vector<Edge>& v);
	SimplePolygon(const SimplePolygon& S);
	~SimplePolygon();
	int inPolygon(Point p);
	std::vector<Point> getVertices();
	std::vector<Edge> getEdges();
};
