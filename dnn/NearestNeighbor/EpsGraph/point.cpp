#include "point.h"
#include "polygon.h"

using namespace std;

Point::Point() { x = 0; y = 0; encl = -1;}

Point::Point(double _x, double _y) { x = _x; y = _y; encl = -1; }

void Point::print() { cout << x << ' ' << y << endl;}

Free_Point::Free_Point() : Point() { host = -1; }

Free_Point::Free_Point(double _x, double _y) { x = _x; y = _y; host = -1; encl = -1;}

Grid_Point::Grid_Point() : Point() { ind = {-1, -1}; num = -1; ip = {false, false, false, false}; encl = -1;}

Grid_Point::Grid_Point(int _row_num, int _col_num, double x_min, double y_max, double eps, int eg_colnum) {
	ind.row = _row_num; ind.column = _col_num;
	x = x_min + ind.column * eps; y = y_max - ind.row * eps;
	num = ind.row * eg_colnum + ind.column;
	encl = -1;

	ip.right = ip.lower = ip.left = ip.upper = false;
}


