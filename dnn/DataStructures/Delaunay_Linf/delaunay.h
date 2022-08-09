#ifndef DELAUNAY_H
#define DELAUNAY_H

#include "./externals/quadedge-basic/quadedge.h"
#include <stdlib.h>
#include <iostream>
#include <vector>

Edge2d* delaunay(vector<Point2d>& S);
Point2d pointToPoint2d(Point& p);
Graph* ptsToDelaunay(const vector<Point>& S);






#endif