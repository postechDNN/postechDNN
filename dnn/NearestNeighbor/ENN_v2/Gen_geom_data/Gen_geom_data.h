#pragma once

#include "../../ENN/C_Subidivision/DCEL/DCEL.h"
#include "../../ENN/C_Subidivision/DCEL/Point.h"
#include "../../ENN/C_Subidivision/DCEL/Polygon.h"
#include "../../ENN/C_Subidivision/DCEL/Graph.h"
#include <random>
#include <cmath>
#include <queue>
#include <tuple>
#include <algorithm>

class Gen_geom_data{
    double min_x, max_x;
    double min_y, max_y;
    std::mt19937 gen;
    static double const_pi(){return std::atan(1)*4;}
public:
    Gen_geom_data(Point left_bottom, Point right_top);
    
    //Generate n random points in a bounding box which follow some distribution
    std::vector<Point> gen_points_uniform(int n);
    std::vector<Point> gen_points_gaussian(int n, Point mean, double dev_x, double dev_y);

    //Generate a random simple polygon in a bounding box, where total number of vertices is n
    std::vector<double> randomAngleSteps(int n, double irregularity); //generate random angle
    double clip(double value, double lower, double upper);//bounding box
    SimplePolygon gen_simple_polygon(int n, double center_x, double center_y);
    //Generate m random disjoint simple polygons in a bounding box, where total number of vertices is n.
    DCEL gen_polygonal_domain(int n);
    std::vector<SimplePolygon> gen_polygonal_domain(int n, int num_polys);
    std::vector<Point> gen_sources(std::vector<SimplePolygon>& obstacles, int n);

    //Generate a planar graph on which the number of vertices is n and the number of edges is m. 
    Graph<Point> gen_planar_graph(int n); 

    
    ~Gen_geom_data();
};