#pragma once
#include <vector>
#include <random>

class Point {
public:
	int n;
	std::vector<double> xs;

public:
	Point();
	Point(int);
	Point(std::vector<double>);
	Point(int, double);
	Point(Point*);
	~Point();

	bool operator==(Point);

	double getx(int);
	void setxs(std::vector<double>);
	std::vector<double> getxs();

	double dist(Point);
};
