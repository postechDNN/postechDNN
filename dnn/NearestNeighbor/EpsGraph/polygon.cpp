#include "polygon.h"

#define X true // ray direction
#define Y false

using namespace std;

Polygon::Polygon() { x_min = x_max = y_min = y_max = 0; encl_pts = {}; ord = -1; }

Polygon::Polygon(vector <Point> _vers, int _ord) {
	vers = _vers;	// vertices are sorted CW 

	double temp_xmax = DBL_MIN, temp_xmin = DBL_MAX;
	double temp_ymax = DBL_MIN, temp_ymin = DBL_MAX;

	for (unsigned int i = 0; i < vers.size(); i++)
	{
		if (_vers[i].x > temp_xmax) { temp_xmax = _vers[i].x; }
		else if (_vers[i].x < temp_xmin) { temp_xmin = _vers[i].x; }

		if (_vers[i].y > temp_ymax) { temp_ymax = _vers[i].y; }
		else if (_vers[i].y < temp_ymin) { temp_ymin = _vers[i].y; }
	}
	x_max = temp_xmax; x_min = temp_xmin;
	y_max = temp_ymax; y_min = temp_ymin;

	encl_pts = {};

	ord = _ord;
}

bool Polygon::intersect(Point p1, Point p2, bool direc) { // filters out easy-to-detect non-intersecting cases
	for (unsigned int i = 0; i < vers.size(); i++)
	{
		unsigned int j = (i + 1) % vers.size();

		double min_one, max_one, the_other, i_one, i_other, j_one, j_other;
		if (direc == X)
		{
			min_one = min(p1.x, p2.x), max_one = max(p1.x, p2.x), the_other = p1.y;
			i_one = vers[i].x, i_other = vers[i].y, j_one = vers[j].x, j_other = vers[j].y;

		}
		else
		{
			min_one = min(p1.y, p2.y), max_one = max(p1.y, p2.y), the_other = p1.x;
			i_one = vers[i].y, i_other = vers[i].x, j_one = vers[j].y, j_other = vers[j].x;
		}

		if (max_one <= min(i_one, j_one) || max(i_one, j_one) <= min_one ||
			the_other <= min(i_other, j_other) || max(i_other, j_other) <= the_other) {
		} // does not intersect
		else {
			if (i_one != j_one) // calculate the slope of the line
			{
				double m = (j_other - i_other) / (j_one - i_one), n = i_other - m * i_one; // needs to check once again
				double inter_one = (the_other - n) / m;
				if (min_one < inter_one && inter_one < max_one) { return true; }
			}
			else {
				if (min_one < i_one && i_one < max_one) { return true; }
			}
		}
	}

	return false;
}

int Polygon::ray(Point p) // shoots a ray from the point to the right. computes the sum of # of intersections with the polygons
{	
	if (x_max < p.x || x_min > p.x || y_max < p.y || y_min > p.y) { return -1; }

	int inter = 0;
	double p_a, p_b, i_a, i_b, j_a, j_b;
	for (unsigned int i = 0; i < vers.size(); i++)
	{
		int j = (i + 1) % vers.size();
		int i_prime = (i - 1) % vers.size();
		int j_prime = (j + 1) % vers.size();
		p_a = p.x; p_b = p.y; i_a = vers[i].x; i_b = vers[i].y; j_a = vers[j].x; j_b = vers[j].y;

		if (max(i_a, j_a) < p_a || p_b < min(i_b, j_b) || max(i_b, j_b) < p_b) {} // inter += 0
		else {
			if (i_a == j_a) {
				if (p_a == i_a) { return -2; }
				else {
					if (min(i_b, j_b) < p_b && p_b < max(i_b, j_b))
					{
						inter += 1;
					}
					else {
						if (p_b == j_b) {
							if ((vers[j_prime].y > vers[j].y && vers[j].y > vers[i].y) ||
								(vers[j_prime].y < vers[j].y && vers[j].y < vers[i].y))
							{
								inter += 1;
							}
							else {} // inter += 0
						}
					} // shape of '¤¡'
				}
			}
			else if (i_b == j_b) {
				if (min(i_a, j_a) <= p_a && p_a < max(i_a, j_a)) { return -2; }
				else {
					if (p_b == j_b) {
						if (vers[j_prime].y > j_b == vers[i_prime].y > i_b) {} // inter += 0
						else { inter += 1; }
					}
				} // shape of a morse code.
			}
			else {
				long double m = (j_b - i_b) / (j_a - i_a), n = i_b - m * i_a;
				long double inter_x = (p_b - n) / m;
				if (inter_x < p_a) {} // inter += 0
				else if (inter_x == p_a) { return -2; }
				else {
					if (p_b != i_b && p_b != j_b)
					{
						inter += 1;
					}
					else {
						if (p_b == j_b) {
							if (vers[j_prime].y > vers[j].y == vers[j].y > vers[i].y)
							{
								inter += 1;
							}
							else {} // inter += 0
						}
					} // shape of '45 degrees'
				}
			}
		}

	}
	return inter;
}

