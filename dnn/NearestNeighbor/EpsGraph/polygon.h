#pragma once

#include "point.h"
#include <cmath>
#include <algorithm>

class Polygon {
private:

public:
	vector<Point> vers; // vertices
	double x_min, x_max, y_min, y_max;	// x_min : minimum among x-coordinates of the polygon vertices. x_max, y_min, y_max are defined similarly.


public:
	Polygon();
	Polygon(vector <Point>);
	bool intersect(Point, Point, bool);	// filters out easy-to-detect non-intersecting cases
	int ray(Point p);	// shoots a ray from the point to the right. computes the sum of # of intersections with the polygons
};

/*
long double get_xmin();
long double get_xmax();
long double get_ymin();
long double get_ymax();
*/

// int ver_num; // # of vertices