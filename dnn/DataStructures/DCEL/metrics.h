#pragma once
#include "./..//..//Algorithms/ESP_plane/Point.h"



double l_1_metric(Point& p_1, Point& p_2);
double l_2_metric(Point& p_1, Point& p_2);
double l_inf_metric(Point& p_1, Point& p_2);
double taxicab_metric(Point& p_1, Point& p_2);
double inner_product(Point& p_1, Point& p_2);
double length(Point& p_1);
double cosine_distance(Point& p_1, Point& p_2);
double weighted_square(Point& p_1, Point& p_2, double upper, double lower, double left, double right);