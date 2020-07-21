#pragma once
//this class defines a point in 2D space

//can use int, double
class Point {
protected:
	double x, y;

public:

	Point();
	Point(double, double);
	Point(Point*);
	~Point();
	bool operator==(Point);
	double getx(void);
	void setx(double);
	double gety(void);
	void sety(double);

	double distance(Point);
};