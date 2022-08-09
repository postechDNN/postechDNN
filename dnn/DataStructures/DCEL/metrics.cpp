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

double weighted_square(Point& p_1, Point& p_2, double upper, double lower, double left, double right)
// p_1을 기준으로 p_2의 거리를 측정
{
	if (p_2.getx() >= p_1.getx() && p_2.gety() >= p_1.gety()) { return std::max(abs(p_1.getx() - p_2.getx()) / right, abs(p_1.gety() - p_2.gety()) / upper); }
	else if (p_2.getx() < p_1.getx() && p_2.gety() >= p_1.gety()) { return std::max(abs(p_1.getx() - p_2.getx()) / left, abs(p_1.gety() - p_2.gety()) / upper); }
	else if (p_2.getx() >= p_1.getx() && p_2.gety() < p_1.gety()) { return std::max(abs(p_1.getx() - p_2.getx()) / right, abs(p_1.gety() - p_2.gety()) / lower); }
	else { return std::max(abs(p_1.getx() - p_2.getx()) / left, abs(p_1.gety() - p_2.gety()) / lower); }
}
