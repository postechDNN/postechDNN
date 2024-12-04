#pragma once

#include "../ENN/C_Subidivision/DCEL/polygon.h"
#include "../ENN/C_Subidivision/DCEL/Vector.h"
#include "../ENN/C_Subidivision/DCEL/DCEL.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <iterator>


using namespace std;

class Arrangement : public DCEL {
protected:
    std::map<std::string, int> he_ver_map; //key of HEdge -> index in vertices
    void make_Rectangle(double, double);
public: 
    Arrangement():DCEL() {};
    Arrangement(int, vector<Point>);
    


};


class Space {
public:
    vector<Point> srcs;
    vector<SimplePolygon> obstacles; // Obstacles
    vector<Point> vertices; // Input points + Obstacle Points
    vector<double> dists;
    vector<bool> visited;
    vector<vector<pair<long long, double>>> adj_list; // Adjacent list
    Arrangement Arr;

public:
    // Setting
    Space(vector<Point>, vector<SimplePolygon>);
    ~Space();
    void set_Space(vector<Point>, vector<SimplePolygon>);


    void visibility_graph();

    // Function
    void add_Polygon(SimplePolygon);
    void del_Polygon(int);
    void add_vert(Point);
    void del_vert(int);
    void Dijkstra(Point query);
    void print_knn(int);
};
