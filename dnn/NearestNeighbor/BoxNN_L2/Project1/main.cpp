#include <iostream>
#include <list>
#include <vector>
#include "Point.h"
#include "Box.h"
#include "Space.h"

int main() {

	Box test(3, { 0,0,0 }, { 1,1,1 });
	vector<Box> boxes;
	boxes.push_back(test);
	Point e1({ 2.,2.,2. });
	Point e2({ -1.,-1.,-1. });
	vector<Point> points;
	points.push_back(e1);
	points.push_back(e2);
	vector<double> temp = { 3.0, 3.0, 3.0 };
	Point a(temp);
	Space space(3, points, boxes, 0.1);

	Point q({ 7., 2., 7. });
	space.query(q);

	//space.knn(a, 1);
	return 0;
}
