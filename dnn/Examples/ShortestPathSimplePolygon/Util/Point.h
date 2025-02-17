#pragma once
typedef double point_type;
using namespace std;

class Point {
private:
	int id;
	point_type x;
	point_type y;
public:
	Point() {
	}
	Point(point_type _x, point_type _y) {
		id = -1;
		x = _x;
		y = _y;
	}
	Point(int _id, point_type _x, point_type _y) {
		id = _id;
		x = _x;
		y = _y;
	}
	int get_id() { return id; }
	
	point_type get_x() {
		return x;
	}
	point_type get_y() {
		return y;
	}
	point_type get_x(bool reverse) {
		if (reverse) return -1 * x;
		else return x;
	}
	point_type get_y(bool reverse) {
		if (reverse) return -1 * y;
		else return y;
	}
	Point get_reverse(bool reverse) {
		if(reverse) return Point(id, -x, -y);
		else return Point(id, x, y);
	}

	bool check_equal(Point _p)
	{
		if (x == _p.get_x() && y == _p.get_y())
			return true;
		else
			return false;
	}
	
};

vector<Point> point_list;