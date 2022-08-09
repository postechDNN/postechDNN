#ifndef DELAUNAY_H
#define DELAUNAY_H

#include "./externals/quadedge-basic/quadedge.h"
#include <stdlib.h>
#include <iostream>
#include <vector>

Edge2d* delaunay(vector<Point2d>& S);
void toDelaunay(vector<Point>& ptS, Graph* G);






#endif