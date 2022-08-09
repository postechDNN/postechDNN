#include "CS.h"
#include <iostream>
#include <tuple>
#include <vector>

using namespace std;

int main() {
	std::vector<Point*> vec = {new Point(24.0 ,24.0), new Point(16.0, 6.0)};
	Point* s = new Point(0.0, 0.0);
	Point* t = new Point(30.0, 0.0);

	Point* p1 = new Point(16.01, -8.01); vec.push_back(p1);
	Point* p2 = new Point(7.9, 0.1); vec.push_back(p2);


	// 	std::vector<i_quad*> vec;
	// i_quad* q1 = new i_quad(3, 16.01, -8.01); vec.push_back(q1);
	// i_quad* q2 = new i_quad(3, 7.9, 0.1); vec.push_back(q2);
	
	auto CS = conforming_subdivision(s, t, vec);
	return 0;
}

