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
    vector<double> max;
    
public:
    Box(int, vector<double>, vector<double>);
    ~Box();

    vector<vector<double>> generate_epsilon(double rmin, double epsilon);
    bool intersect(Point, Point);
};
