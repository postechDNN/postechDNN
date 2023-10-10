#pragma once

#include "../../DCEL/Point.h"
#include "../../DCEL/Polygon.h"
#include "../../DCEL/Graph.h"
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
    SimplePolygon gen_simple_polygon(int n);

    //Generate a planar graph on which the number of vertices is n and the number of edges is m. 
    Graph<Point> gen_planar_graph(int n); 
    
    ~Gen_geom_data();
};