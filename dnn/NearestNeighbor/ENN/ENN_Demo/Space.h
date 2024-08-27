#pragma once

#include "../C_Subidivision/DCEL/polygon.h"
#include "../C_Subidivision/DCEL/point.h"
#include "../C_Subidivision/DCEL/Vector.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <iterator>


using namespace std;

class Space {
public:
    vector<Point> srcs;
    vector<SimplePolygon> obstacles; // Obstacles
    vector<Point> vertices; // Input points + Box vertices + Steiner points
    vector<double> dists;
    vector<bool> visited;
    vector<vector<pair<long long, double>>> adj_list; // Adjacent list

public:
    // Setting
    Space(vector<Point>, vector<SimplePolygon>);
    ~Space();
    void set_Space(int, vector<Point>, vector<SimplePolygon>);

    vector<pair<double, double>> Combination();
    void cal_rmin();

    void visibility_graph();

    // Function
    void add_Box(SimplePolygon);
    void del_Box(int);
    void add_vert(Point);
    void del_vert(int);
    void Dijkstra(Point query);
    void print_knn(int);
};
