#pragma once

#include "point.h"
#include <cmath>
#include <algorithm>

class Polygon {
private:

public:
	vector<Point> vers; // vertices
	double x_min, x_max, y_min, y_max;	// x_min : minimum among x-coordinates of the polygon vertices. x_max, y_min, y_max are defined similarly.
	vector<Point> encl_pts; // free/grid points that are enclosed by the polygon
	int ord; // represents index

public:
	Polygon();
	Polygon(vector <Point>, int);
	bool intersect(Point, Point, bool);	// checks if a polygon edge crosses the line connecting two adjacent grid points (if so, they should be disconnected)
	int ray(Point p);	// shoots a ray from the point to the right. computes the sum of # of intersections with the polygon
};

