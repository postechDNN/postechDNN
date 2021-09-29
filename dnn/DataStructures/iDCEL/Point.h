#pragma once

class Point {
protected:
	long long int x, y;

public:
	Point(long long int,long long int);
	Point(Point*);
	~Point();
	bool operator==(Point);
	Point operator- (Point);
	bool operator<=(Point);
	long long int getx(void);
	long long int gety(void);
};
