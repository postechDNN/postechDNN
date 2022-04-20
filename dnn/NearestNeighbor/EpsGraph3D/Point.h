#pragma once

class Point{};

class Grid_Point : public Point {
public:
	indices ind;	// its location on the grid
	incid_pts ip;
	int num;

	vector<Free_Point*> anchored;	// free points anchored to it

	vector<int> cros; // crossings

public:
	Grid_Point();
	Grid_Point(int, int, int, double, double, double, double, int);
};