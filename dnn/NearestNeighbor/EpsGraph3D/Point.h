#pragma once

struct indices {
	int row;
	int column;
	int layer;
};

class Point {
protected:
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

class Grid_Point : public Point {
public:
	indices ind;	// its location on the grid
	int num;

	vector<Free_Point*> anchored;	// free points anchored to it

	vector<int> cros; // crossings

public:
	Grid_Point();
	Grid_Point(int, int, int, double, double, double, double, int);
};