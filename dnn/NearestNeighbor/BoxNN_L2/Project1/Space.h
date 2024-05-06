#pragma once

#include "Box.h"
#include "point.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <iterator>


using namespace std;

class Space {
public:
    int d; // dimension
    double epsilon;// approximation factor
    double rmin;
    vector<Box> Boxes; // Obstacles
    vector<Point> vertices; // Input points + Box vertices + Steiner points
    vector<vector<pair<long long int, double>>> adj_list; // Adjacent list

public:
    // Setting
    Space(int, vector<Point>, vector<Box>);
    ~Space();
    void set_Space(int, vector<Point>, vector<Box>);

    vector<pair<double, double>> Combination();
    vector<vector<double>> gen_SteinerPoint();
    void cal_rmin();

    void visibility_graph();

    // Function
    void add_Box(Box);
    void del_Box(int);
    void add_vert(Point);
    void del_vert(int);
    void Dijkstra(Point query);
    void print_knn(int);
};
