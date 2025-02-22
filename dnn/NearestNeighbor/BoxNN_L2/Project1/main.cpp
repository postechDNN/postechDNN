#include <iostream>
#include <list>
#include <vector>
#include "Point.h"
#include "Box.h"
#include "Space.h"
#include "data_gen.h"
#include "Space_IO.h"

int main() {
	vector<Box> boxes;
	vector<Point> points;
	//generate_data(3, 5, 5, "input.txt");
	Point q({ 10.,10.,20. });
	text_input(points, boxes, q,"input.txt");
	BoxGraphL2 space(3, points, boxes, 0.1);
	auto result = space.query(q);
	cout << "Point: " << get<0>(result).getx(0) << " " << get<0>(result).getx(1) << " " << get<0>(result).getx(2) << endl;
	cout << "dist: " << get<2>(result) << endl;
	print_result(space, q, result, "output.txt");
	//space.knn(a, 1);
	return 0;
}
