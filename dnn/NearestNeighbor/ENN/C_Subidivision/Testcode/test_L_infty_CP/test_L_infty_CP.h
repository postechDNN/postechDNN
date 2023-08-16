#pragma once
#include <vector>
#include "../../DCEL/Point.h"
#include <limits>

bool test_L_infty_CP(std::vector<Point> &, double);
double inline L_infty_dist(Point&, Point&);