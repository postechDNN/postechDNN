#pragma once
#include <vector>

class Point {
public:
	int n;
	std::vector<double> xs;
	Point();
	Point(int);
	Point(std::vector<double>);
	Point(Point*);
	~Point();
	bool operator==(Point);
	double getx(int);
	void setxs(std::vector<double>);
	std::vector<double> getxs();
};
