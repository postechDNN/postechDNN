#pragma once

class Point{
protected:
	double x, y, z;

public:

	Point();
	Point(double, double, double);
	Point(Point*);
	~Point();
	bool operator==(Point);
	Point operator- (Point);
	double getx(void);
	void setx(double);
	double gety(void);
	void sety(double);
	double getz(void);
	void setz(double);

	double distance(Point);
};