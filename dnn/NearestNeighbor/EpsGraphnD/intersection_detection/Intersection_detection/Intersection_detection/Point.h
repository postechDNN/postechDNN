#pragma once
#include <vector>
#include "stdafx.h"
using namespace std;

/*struct indices {
	int n;
	vector<long long int> ind;
};

struct ind_pts {
	int n;
	vector<bool> topmost;
	vector<bool> bottommost;
};*/

class Point {
public:
	int n;
	std::vector<double> xs;

public:
	Point(int);
	Point(std::vector<double>);
	Point(Point*);
	~Point();
	bool operator==(Point);
	Point operator- (Point);
	double getx(int);
	void setx(int, double);
	std::vector<double> getxs();
	void setxs(std::vector<double>);
	int getsize();
	double distance(Point);
};

class Free_Point : public Point {
public:
	Free_Point(std::vector<double>);
	int host = -1;
};

class Grid_Point : public Point {
public:
	vector<long long int> ind;	// its location on the grid
	vector<bool> topmost;
	vector<bool> bottommost;
	int num;
	int encl;

	std::vector<Free_Point*> anchored;	// free points anchored to it


public:
	Grid_Point(int);
	Grid_Point(int, vector<long long int>, vector<double>, vector<int>);
};