#pragma once
#include <cassert>
#include "Point.h"
#include <vector>

class EucGraph{ //unweighted graph
private:
    int n_vertices;
    int n_edges;
    std::vector<Point> vertices;
    std::vector<std::vector<int> > edges;
public:
    EucGraph();
    ~EucGraph();
    int size_vertices();
    int size_edges();
    int insert_vertex(Point); //O(1)
    void insert_edge(int, int); //O(1)
    Point getVertex(int); //O(1)
    bool is_connected(int,int); //O(E)
    std::vector<int> getAdjlist(int); //O(E_v) where E_v is the number of edges adjacent to the vertex 
    std::vector<std::vector<int> > getAdjlist();
    std::vector<Point> getVertices(); // O(V) where V is the number of vertices
};