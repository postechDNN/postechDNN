#include <iostream>
#include <list>
#include <vector>
#include "Boxnn.h"
#include "Point.h"
#include "Box.h"
#include "Space.h"

int main() {

	Box test(3, { 0,0,0 }, { 1,1,1 });
	vector<Box> boxes;
	boxes.push_back(test);
	vector<double> temp = { 3.0, 3.0, 3.0 };
	Point a(temp);
	vector<Point> points;
	points.push_back(a);
	Point e1({-0.5,-0.5,-0.5});
	Point e2({3,3,3});
	cout << test.intersect(e1, e2)<<endl;
	//Space space(3, points, boxes);



	return 0;
}
