#pragma once

#include "point.h"
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class Box {
public:
    int d;
    vector<double> min;
    //min=vertices.first;
    vector<double> max;
    
public:
    Box(int, vector<double>, vector<double>);
    ~Box();

    vector<vector<double>> generate_epsilon(double rmin, double epsilon);
    bool intersect(Point, Point);
};

/*
int d; // dimension
double epsilon;// approximation factor
double rmin;
vector<Point> vertices;
*/

//vector<vector<double>> generate_epsilon(vector<double> &min, vector<double> &max, double rmin, double epsilon);
  //
// bool Intersect(Point, Point);
//std::vector<std::pair<Point, Point>> d_axis(int d, Box vertex_1, Box vertex_2);