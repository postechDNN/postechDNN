#pragma once
#include "Point.h"

double l_1_metric(Point& p_1, Point& p_2);
double l_2_metric(Point& p_1, Point& p_2);
double l_inf_metric(Point& p_1, Point& p_2);
double taxicab_metric(Point& p_1, Point& p_2);
double inner_product(Point& p_1, Point& p_2);
double length(Point& p_1);
double cosine_distance(Point& p_1, Point& p_2);
double min_weighted(Point& p_1, Point& p_2, double, double);
double max_weighted(Point& p_1, Point& p_2, double, double);