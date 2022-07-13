#include <string>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "dots.h"
#include "propagation.h"
#include "steiner_pts.h"
#include "parser.h"

int main() {
	//PolyDomain D = BuildPolyDomain("cube.1");
	PolyDomain D = BuildPolyDomain("test.1");
	D.Initialize();
	Point p0(0.9, 0.9, 0.8);
	vector<Point> pts = { Point(0.9, 0.9, 1.2) };
	double dist = D.Nearest_Dist(p0, pts);
	std::cout << "apx_dist :" << dist << endl;

	/*
	srand((int)time(NULL));
	for (int j = 0; j < 10; j++) {
		vector<Point> pts = {};
		for (int i = 0; i < 100; i++) {
			int num1 = rand(), num2 = rand(), num3 = rand();
			pts.push_back(Point(1 / (double(num1) + 1), 1 / (double(num2) + 1), 1 / (double(num3) + 1)));
		}
		std::cout << "apx_dist :" << D.Nearest_Dist(p0, pts) << endl;

		double val = std::numeric_limits<double>::max();
		for (auto pt : pts) {
			double temp = PointsDist(pt, p0);
			if (temp < val) { val = temp; }
		}
		std::cout << "real_dist :" << val << endl;
	}
	*/

	return 0;
}