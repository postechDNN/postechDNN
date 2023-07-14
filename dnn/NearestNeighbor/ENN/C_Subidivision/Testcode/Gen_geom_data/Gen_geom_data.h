#pragma once

#include "../../DCEL/Point.h"
#include "../../DCEL/Polygon.h"

class Gen_geom_data{
    double min_x, max_x;
    double min_y, min_y;
public:
    Gen_geom_data(Point left_bottom, Point right_top);
    //Generate n random points in a bounding box which follow some distribution
    std::vector<Point> gen_points_uniform(int n);
    std::vector<Point> gen_points_gaussian(int n, Point mean, double dev_x, double dev_y);

    //Generate n random simple polygons in a bounding box
    std::vector<SimplePolygon> gen_simple_polygons(int n, int m);
    ~Gen_geom_data();
};