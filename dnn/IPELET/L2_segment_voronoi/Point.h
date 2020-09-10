#pragma once

class Point {
private:
	double pos[2]; // x, y coordinate

public:
	Point();
	Point(double x, double y);
	Point(Point*);
	~Point();

	void setX(double x);
	void setY(double y);
	void setPos(double x, double y);

	double getX();
	double getY();

	double computeDist(Point p);

	void translation(double origin[]);
	void rotation(double radian);

	int ccw(Point b, Point c); // -1 == CW, 0 == colinear, 1 == CCW

	Point* getMedian(Point p);

	Point operator- (Point operand);
	bool operator == (Point operand);
	bool operator !=(Point operand);
	Point operator *(double x);	//Scalar
	double operator *(Point op); //Vector
	double dot_product(Point operand);
	Point operator+ (Point operand);
	bool operator< (Point operand);
	double norm();
	Point operator =(Point p);

};