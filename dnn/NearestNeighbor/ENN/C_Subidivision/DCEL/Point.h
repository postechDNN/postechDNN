#pragma once
#include <iostream>
//this class defines a point in 2D space
constexpr double ERR = 1e-6;

//can use double
class Point {
protected:
	double x, y;

public:

	Point();
	Point(double, double);
	Point(const Point&);
	~Point();
	bool operator==(Point);
	Point operator- (Point);
	double getx(void) const;
	void setx(double);
	double gety(void) const;
	void sety(double);
	double distance(Point);
	double weight;

	friend std::ostream& operator<<(std::ostream& os, const Point& p); //For testing

	void operator=(Point);
};
