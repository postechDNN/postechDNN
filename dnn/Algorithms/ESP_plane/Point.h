#pragma once
#include <cmath>

class Point {
private:
	double x;
	double y;
	int index;
	bool is_src; // if a Point is neither src nor dst, it is an obstacle vertex.
	bool is_dst;
	int edge_type; // ¤¡ - 0, ¤¡ after x-symmetric transformation - 1, 
	                        // ¤¡ after y-symmetric transformation - 2, ¤¤ - 3, 
							// ¤Ñ - 10, ¤Ó - 11, no edges - -1.

public:
	Point();
	~Point();
	Point(double, double);
	Point(double _x, double _y, int i);
	Point(int _x, int _y, int lv);
	friend double operator-(Point& a, Point& b);
	double getX();
	double getY();
	void setIndex(int); 
	int getIndex(); // for s and obstacle vertices
	void print();
	void setEdgeType(int);
	int getEdgeType();
};

bool sortPtbyXIncr(Point& a, Point& b);
bool sortPtbyYDecr(Point& a, Point& b);
double distance(Point* a, Point* b);
int getGridIndex(double, int);
