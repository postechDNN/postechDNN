#pragma once
#include <vector>

struct indices {
	int row;
	int column;
	int layer;
};

struct ind_pts {
	bool right;
	bool lower;
	bool left;
	bool upper;
	bool higher;
	bool deeper;
};

class Point {
public:
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

class Free_Point : public Point {
	int host = -1;
};

class Grid_Point : public Point {
public:
	indices ind;	// its location on the grid
	ind_pts ip;
	int num;
	int encl;

	std::vector<Free_Point*> anchored;	// free points anchored to it


public:
	Grid_Point();
	Grid_Point(int, int, int, double, double, double, double, int, int);
};