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
    vector<Point> sites;
    // Initializing obstacle space
    Space(int, vector<Point>&, vector<Box>&, double _epsilon);
    ~Space();
    void set_Space(int, vector<Point>&, vector<Box>&);

    // Function
    void add_Box(Box);
    void del_Box(int);
    void add_vert(Point);
    void del_vert(int);
    void Dijkstra(Point);
    void print_knn(Point, int);

private:
    vector<Point> vertices; // Input points + Box vertices + Steiner points
    vector<vector<pair<long long int, double>>> adj_list; // Adjacent list
    vector<bool> visited;
    vector<double> dists;
    vector<pair<int,int>> Combination();
    vector<Point> gen_SteinerPoint();
    void cal_rmin();

    void visibility_graph();

    
};
