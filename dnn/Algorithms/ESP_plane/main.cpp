
#include "CS.h"
#include <iostream>
#include <tuple>
#include <vector>

using namespace std;

int main() {
	std::vector<Point*> vec = {new Point(100.1, 100.1), new Point(16.1, 6.1)};
	Point* s = new Point(0.1, 0.1);
	Point* t = new Point(30.1, 2.1);

	Point* p1 = new Point(16.01, -8.01); vec.push_back(p1);
	Point* p2 = new Point(7.9, 0.4); vec.push_back(p2);


	// 	std::vector<i_quad*> vec;
	// i_quad* q1 = new i_quad(3, 16.01, -8.01); vec.push_back(q1);
	// i_quad* q2 = new i_quad(3, 7.9, 0.1); vec.push_back(q2);
	
	auto CS = conforming_subdivision(s, t, vec);
	DCEL D = CS.build_subdivision();
	return 0;
}
