#include "metrics.h"
#include <math.h>
#include <algorithm>

double l_p_metric(Point& p_1, Point& p_2, int p)
{
	return sqrt(pow(p_1.getx() - p_2.getx(), p) + pow(p_1.gety() - p_2.gety(), p));
}

double l_inf_metric(Point& p_1, Point& p_2)
{
	return std::max(p_1.getx() - p_2.getx(), p_1.gety() - p_2.gety());
}

double discrete_metric(Point& p_1, Point& p_2)
{
	if (p_1 == p_2) { return 0; }
	else { return 1; }
}

double taxicab_metric(Point& p_1, Point& p_2)
{
	return abs(p_1.getx() - p_2.getx()) + abs(p_1.gety() - p_2.gety());
}

double inner_product(Point& p_1, Point& p_2)
{
	return p_1.getx() * p_2.getx() + p_1.gety() * p_2.gety();
}

double length(Point& p_1)
{
	return sqrt(pow(p_1.getx(), 2) + pow(p_1.gety(), 2));
}

double cosine_distance(Point& p_1, Point& p_2)
{
	return 1 - inner_product(p_1, p_2) / (length(p_1) * length(p_2));
}

double min_weighted(Point& p_1, Point& p_2, double a, double b)
{
	return std::min(abs(p_1.getx() - p_2.getx()) / a, abs(p_1.gety() - p_2.gety()) / b);
}

double max_weighted(Point& p_1, Point& p_2, double a, double b)
{
	return std::max(abs(p_1.getx() - p_2.getx()) / a, abs(p_1.gety() - p_2.gety()) / b);
}
