#ifndef __POINT_H__
#define __POINT_H__

#include <iostream>
//this class defines a point in 2D space

class Point {
protected:
	double x, y;

public:
	Point();
	Point(double, double);
	Point(Point*);
	~Point();
	double getx(void);
	double gety(void);
	double distance(Point);

	//Jaehoon
	Point operator- (Point operand);
	bool operator == (Point operand);
	bool operator !=(Point operand);
	Point operator *(double x);	//Scalar
	double operator *(Point op); //Vector
	friend std::ostream& operator<<(std::ostream& os, const Point& p);
	double dot_product(Point operand);
	Point operator+ (Point operand);
	bool operator< (Point operand);
	double norm();
};

#endif