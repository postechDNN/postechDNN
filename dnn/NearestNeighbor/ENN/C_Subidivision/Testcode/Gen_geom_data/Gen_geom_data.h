#pragma once

#include "../../DCEL/Point.h"
#include "../../DCEL/Polygon.h"
#include <random>

class Gen_geom_data{
    double min_x, max_x;
    double min_y, max_y;
    std::mt19937 gen;
public:
    Gen_geom_data(Point left_bottom, Point right_top);
    //Generate n random points in a bounding box which follow some distribution
    std::vector<Point> gen_points_uniform(int n);
    std::vector<Point> gen_points_gaussian(int n, Point mean, double dev_x, double dev_y);

    //Generate n random simple polygons in a bounding box, where total number of vertices is m
    std::vector<SimplePolygon> gen_polygons_uniform(int n, int m);
    std::vector<SimplePolygon> gen_polygons_gaussian(int n, int m);
    ~Gen_geom_data();
};