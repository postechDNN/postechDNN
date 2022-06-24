#pragma once
#include <vector>

struct indices {
	int x_ind;
	int y_ind;
	int z_ind;
};

struct ind_pts {
	bool x_u;
	bool x_d;
	bool y_u;
	bool y_d;
	bool z_u;
	bool z_d;
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
public:
	Free_Point(double, double, double);
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