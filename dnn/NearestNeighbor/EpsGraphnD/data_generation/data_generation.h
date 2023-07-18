#pragma once

#include "../algorithm/Point.h"

typedef struct halfplane {

	int d; // dimension
	std::vector<double> vals; // (d+1) values to represent the halfplane. For example, (d=2) ax + by + c = 0, (d=3) ax + by + cz + d = 0.

} halfplane;

enum DIST_FLAG {
	UNIFORM,
	NORMAL,
};

halfplane* hp_thru_p(Point* center);
void x_slice();
std::vector<Point*> brute_force(Point* center, double bound, int num, halfplane* hp, bool b);
std::vector<Point*> pts_on_sphere(int num_pts, Point* center, double radius, DIST_FLAG DF);
double hp_result(halfplane* hp, Point* p);