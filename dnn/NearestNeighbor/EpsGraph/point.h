#pragma once

#include <vector>
#include <functional>
#include <queue>
#include <iostream>

using namespace std;

class Polygon;

struct indices
{
	int row;
	int column;
};

struct incid_pts { // shows if a gridpoint has edges with adjacent ones
	bool right;		
	bool lower;
	bool left;
	bool upper;
};

class Point {
public:	
	double x, y; // x- and y- coordinates
	int encl; // tells if it lies on the interior of an obstacle polygon

public:
	Point();
	Point(double, double);
	bool operator==(const Point& other) const {
		return !(*this == other);
	}
	void print();
};

class Free_Point : public Point { // is the class representing points in the point set P (= non-grid points)
public:
	int host; // denotes the index of a gridpoint on which it cast anchor.
			  // if the free point is in the interior of an obstacle polygon, host = -1

public:
	Free_Point();
	Free_Point(double, double);
};

class Grid_Point : public Point {
public:
	indices ind;	// its matrix-index on the grid 
	incid_pts ip;
	int num;

	vector<Free_Point*> anchored;	// free points anchored to it
	vector<int> cros; // # of crossings of the ray with each polygon. The ray moves rightward originating from the grid point.

public:
	Grid_Point();
	Grid_Point(int, int, double, double, double, int);
};

