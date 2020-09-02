#pragma once

#include "point.h"
#include <cmath>
#include <algorithm>

class _Polygon {
private:

public:
	vector<Point> vers; // vertices
	double x_min, x_max, y_min, y_max;	// x_min : minimum among x-coordinates of the polygon vertices. x_max, y_min, y_max are defined similarly.
	vector<Point> encl_pts;
	int ord;

public:
	_Polygon();
	_Polygon(vector <Point>, int);
	bool intersect(Point, Point, bool);	// filters out easy-to-detect non-intersecting cases
	int ray(Point p);	// shoots a ray from the point to the right. computes the sum of # of intersections with the polygons
};

