#include "CS.h"
#include <iostream>

using namespace std;

int main() {
	vector<Point> pts = { Point{5.875, 2.125}, Point{4.125, 3.875}, Point{1.375, 4.625}, Point{3.625, 6.875}, Point{5.375, 5.625} };
	conforming_subdivision CS(pts);
	CS.build_subdivision();
	/*
	for (line_segment line : CS.BD) {
		cout << "P1 (" << line.p1.x << line.p1.y << ") P2 (" << line.p2.x << line.p2.y << ")" << endl;
	}
	*/
}

/*
	vector<Point> pts = {Point{5.875, 2.125}, Point{4.125, 3.875}, Point{1.375, 4.625}, Point{3.625, 6.875}, Point{5.375, 5.625} };
	conforming_subdivision CS(pts);
	vector<line_segment> lines = CS.build_subdivision();
	for (line_segment line : lines) {
		cout << "P1 (" << line.p1.x << line.p1.y << ") P2 (" << line.p2.x << line.p2.y << ")" << endl;
	}
	*/