#include <iostream>
#include <list>
#include <vector>
#include "Boxnn.h"
#include "Point.h"
#include "Box.h"
#include "Space.h"

int main() {
	/*std::cout << "Hello World" << std::endl;
	std::vector<std::pair<Point, Point> > boxes;
	std::vector<Point> pts;
	Point* b11 = new Point({0., 0., 0.});
	Point* b12 = new Point({1.,1.,1.});
	std::pair<Point, Point> b1 = std::make_pair(b11,b12);
	boxes.push_back(b1);
	Point* b21 = new Point({2.,2.,2.});
	Point* b22 = new Point({3.,4.,5.});
	std::pair<Point, Point> b2 = std::make_pair(b21,b22);
	boxes.push_back(b2);
	Point* p1 = new Point({1.,0.,0.});
	pts.push_back(p1);
	Point* p2 = new Point({2.,1.,4.});
	pts.push_back(p2);
	Point* p3 = new Point({0.,2.,5.});
	pts.push_back(p3);
	Point* q1 = new Point({-1.,-1.,-1.});
	std::cout << "Hello World";*/
	
	//std::vector<Space> B1;
	Box test(3, { 0,0,0 }, { 1,1,1 });
	vector<Box> boxes;
	boxes.push_back(test);
	vector<double> temp = { 3.0, 3.0, 3.0 };
	Point a(temp);
	vector<Point> points;
	points.push_back(a);
	Space space(3, points, boxes);



	return 0;
}
