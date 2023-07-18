#pragma once
#include <vector>

/*
struct indices {
	long long int x_ind;
	long long int y_ind;
	long long int z_ind;
};

struct ind_pts {
	bool x_u;
	bool x_d;
	bool y_u;
	bool y_d;
	bool z_u;
	bool z_d;
};
*/

class Point {
public:
	int n;
	std::vector<double> xs;
	//double x, y, z;

public:
	Point();
	Point(int);
	//Point(double, double, double);
	Point(std::vector<double>);
	Point(Point*);
	~Point();
	bool operator==(Point);
	Point operator- (Point);
	double getx(int);
	void setx(int, double);
	std::vector<double> getxs();
	void setxs(std::vector<double>);
	//double gety(void);
	//void sety(double);
	//double getz(void);
	//void setz(double);
	int getsize();
	double distance(Point);
};

class Free_Point : public Point {
public:
	Free_Point(std::vector<double>);
	int host = -1;
	std::vector<double> xs_min;
	std::vector<double> xs_max;
};

class Grid_Point : public Point {
public:
	std::vector<long long int> ind;	// its location on the grid
	std::vector<bool> ip;
	int num;
	int encl;

	std::vector<Free_Point*> anchored;	// free points anchored to it


public:
	Grid_Point(int);
	Grid_Point(std::vector<long long int>, Point, double, std::vector<long long int>);
	// Grid_Point(int, int, int, double, double, double, double, int, int);
};