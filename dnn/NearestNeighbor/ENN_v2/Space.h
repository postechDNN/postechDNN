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
    std::vector<Vertex*> start_verts;
    void make_Rectangle(double, double);
public: 
    Arrangement():DCEL() {};
    Arrangement(const vector<Point>&);
};

class SimplePolygon_S : public SimplePolygon {
public:
    int index;

    SimplePolygon_S(const SimplePolygon& S):SimplePolygon(S) {};
    ~SimplePolygon_S() {};
};

class Edge_S : public Edge {
public:
    int poly;
    Edge_S(const Edge& E):Edge(E) {};
    ~Edge_S() {};
};

class Point_S : public Point {
public:
    int index;
    int edge_s = -1;
    int edge_t = -1;
    int poly = -1;

    Point_S(const Point& E) :Point(E) {};
    ~Point_S() {};
};

class VisGraph {
public:
    vector<Point_S> srcs; //Sources
    vector<SimplePolygon_S> obstacles; // Obstacles
    vector<Edge_S> edges; // Obstacles' edges
    vector<Point_S> vertices; // Sources + Obstacle Points
    vector<double> dists; //distance from the nearest source
    vector<int> near_src; //index of the nearest source
    vector<bool> visited;
    vector<vector<pair<int, double>>> adj_list; // Adjacent list
    Arrangement arr;

public:
    // Setting
    VisGraph(const vector<Point>&, const vector<SimplePolygon>&);
    ~VisGraph();
    void set_Space(const vector<Point>&, const vector<SimplePolygon>&);


    void visibility_graph();

    // Function
    void add_Polygon(SimplePolygon);
    void del_Polygon(int);
    void add_vert(Point);
    void del_vert(int);
    void Dijkstra();
    tuple<Point,int, double> query(Point query);
};

