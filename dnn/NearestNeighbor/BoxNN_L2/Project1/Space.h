#pragma once

#include "Box.h"
#include "point.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <tuple>


using namespace std;

class BoxGraphL2 {
public:
    int d; // dimension
    double epsilon;// approximation factor
    double rmin;
    vector<Box> Boxes; // Obstacles
    vector<Point> sites;
    BoxGraphL2(int, vector<Point>&, vector<Box>&, double _epsilon);    // Initializing obstacle space
    ~BoxGraphL2();
    void set_Space(int, vector<Point>&, vector<Box>&);

    // Function
    void add_Box(Box);
    void del_Box(int);
    void add_vert(Point);
    void del_vert(int);
    void Dijkstra();
    tuple<Point, int, double>  query(Point);


    vector<Point> vertices; // Input points + Box vertices + Steiner points
    vector<vector<pair<long long int, double>>> adj_list; // Adjacent list
    vector<double> dists; //distance from the nearest source
    vector<int> near_src; //index of the nearest source
private:
    vector<bool> visited;
    vector<pair<int,int>> Combination();
    vector<Point> gen_SteinerPoint();
    void cal_rmin();

    void visibility_graph();
    

    
};
