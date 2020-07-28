#pragma once

#include <vector>
#include <functional>
#include <queue>
#include <iostream>

using namespace std;

struct indices	// 이건 묶고, right와 lower는 그대로 두는 게 제일 낫네.
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

public:	// functions
	Point();
	Point(double, double);
	bool operator==(const Point& other) const {	// 오버로딩 제대로 됐나?
		return !(*this == other);
	}
	void print();
};

class Free_Point : public Point {
	// 딱히 Point에서 더 추가할 건 없지만, gridpoint와의 확실한 구별을 위해. 나중에 NN 추가할 수도 있고.
public:
	double dist; // distance to the nearest gridpoint
	// Grid_Point host;

public:
	Free_Point();
	Free_Point(double, double);
};

struct cmp {
	bool operator()(Free_Point fp1, Free_Point fp2) {
		return fp1.dist < fp2.dist;
	}
};

class Grid_Point : public Point {
public:
	priority_queue <Free_Point, vector<Free_Point>, cmp> pq;
	indices ind;	// its location on the grid
	// vector<Free_Point> acd_pts;	// free points anchored to it
	incid_pts ip;
	int num;


public:
	Grid_Point();
	Grid_Point(int, int, double, double, double, int);
};

/*
	long double getx();
	long double gety();
	int get_row();
	int get_col();
*/