#pragma once
#include <vector>
#include "dots.h"

using namespace std;


class Segment {
private:
	Point a,b;
	vector<double> X;
public:
	Segment(Point _a, Point _b, vector<double> _X) {
		a = _a;
		b = _b;
		X = vector<double>(_X);
	}
};

