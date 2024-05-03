#pragma once

#include "point.h"
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class Space;

class Box {
public:
    int d;
    vector<Point> min;
    //min=vertices.first;
    vector<Point> max;
    
public:
    
    std::vector< std::vector<double>> generate_epsilon(vector<double> &min, vector<double> &max, 
        Space* space_a, double epsilon);
    //
    Box(int, std::vector<Point>);
    ~Box();
   
};

/*
int d; // dimension
double epsilon;// approximation factor
double rmin;
vector<Point> vertices;
*/


// bool Intersect(Point, Point);
//std::vector<std::pair<Point, Point>> d_axis(int d, Box vertex_1, Box vertex_2);