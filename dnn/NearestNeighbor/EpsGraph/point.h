#pragma once

#include <vector>
#include <functional>
#include <queue>
#include <iostream>

using namespace std;

class Polygon;

struct indices	// �̰� ����, right�� lower�� �״�� �δ� �� ���� ����.
{
	int row;
	int column;
};

struct incid_pts {
	bool right;		// to represent edges
	bool lower;
	bool left;
	bool upper;
};

class Point {
public:	// variables
	double x, y;	// x- and y- coordinates
	int encl; // tells if it lies on the interior of an obstacle polygon

public:	// functions
	Point();
	Point(double, double);
	bool operator==(const Point& other) const {	// �����ε� ����� �Ƴ�?
		return !(*this == other);
	}
	void print();
};

class Free_Point : public Point {
	// ���� Point���� �� �߰��� �� ������, gridpoint���� Ȯ���� ������ ����. ���߿� NN �߰��� ���� �ְ�.
public:
	int host; // denotes gridpoint hosting it

public:
	Free_Point();
	Free_Point(double, double);
};

class Grid_Point : public Point {
public:
	indices ind;	// its location on the grid
	incid_pts ip;
	int num;

	vector<Free_Point*> anchored;	// free points anchored to it

	vector<int> cros; // crossings

public:
	Grid_Point();
	Grid_Point(int, int, double, double, double, int);
};

