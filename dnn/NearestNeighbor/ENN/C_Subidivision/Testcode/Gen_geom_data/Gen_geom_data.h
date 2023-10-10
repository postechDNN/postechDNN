#pragma once

#include "../../DCEL/Point.h"
#include "../../DCEL/Polygon.h"
#include "../../DCEL/Graph.h"
#include <random>
#include <cmath>
#include <queue>
#include <tuple>
#include <algorithm>

class Gen_geom_data{
    double min_x, max_x;
    double min_y, max_y;
    std::mt19937 gen;
    static constexpr double const_pi(){return std::atan(1)*4;}
    std::vector<double> randomAngleSteps(int n);
    double clip(double value, double lower, double upper);
public:
    Gen_geom_data(Point left_bottom, Point right_top);
    
    //Generate n random points in a bounding box which follow some distribution
    std::vector<Point> gen_points_uniform(int n);
    std::vector<Point> gen_points_gaussian(int n, Point mean, double dev_x, double dev_y);

    //Generate a random simple polygon in a bounding box, where total number of vertices is n
    SimplePolygon gen_simple_polygon(int n);
    //Generate m random disjoint simple polygons in a bounding box, where total number of vertices is n.
    std::vector<SimplePolygon> gen_polygonal_domain(int n, int num_polys);

    //Generate a planar graph on which the number of vertices is n and the number of edges is m. 
    Graph<Point> gen_planar_graph(int n); 
    
    ~Gen_geom_data();
};