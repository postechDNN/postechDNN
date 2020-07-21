#include "point.h"

using namespace std;

Point::Point() { x = 0; y = 0;}

Point::Point(double _x, double _y) { x = _x; y = _y; }

void Point::print() { cout << x << ' ' << y << endl;}

Free_Point::Free_Point() : Point() { dist = -1.0; }

Free_Point::Free_Point(double _x, double _y) { x = _x; y = _y; dist = -1.0;}

Grid_Point::Grid_Point() : Point() { ind = {-1, -1}; num = -1; ip = {false, false, false, false}; }
// num = -1; ind = {-1, -1}; ip = {};

Grid_Point::Grid_Point(int _row_num, int _col_num, double x_min, double y_max, double eps, int eg_colnum) {
	ind.row = _row_num; ind.column = _col_num;
	x = x_min + ind.column * eps; y = y_max - ind.row * eps;
	num = ind.row * eg_colnum + ind.column;

	ip.right = ip.lower = ip.left = ip.upper = false;
}


// dist = -1;