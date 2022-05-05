#include "EucGraph.h"

EucGraph::EucGraph(){
    this->n_edges = this->n_vertices = 0;
}

EucGraph::~EucGraph(){}

int EucGraph::size_vertices(){
    return this->n_vertices;
}
int EucGraph::size_edges(){
    return this->n_edges;
}
int EucGraph::insert_vertex(Point p){
    int key = this->n_vertices;
    this->n_vertices++;
    this->vertices.push_back(p); 
    this->edges.push_back(std::vector<int>());
    return key;
}

void EucGraph::insert_edge(int k1, int k2){
    assert(k1 < this->n_vertices && k2 < this->n_vertices);
    this->n_edges++;
    this->edges[k1].push_back(k2);
    this->edges[k2].push_back(k1);
}

Point EucGraph::getVertex(int k){
    assert(k < this->n_vertices);
    return this->vertices[k];
}
bool EucGraph::is_connected(int k1,int k2){
    assert(k1 < this->n_vertices && k2 < this->n_vertices);
    for(auto k:edges[k1]){
        if(k == k2) return true;
    }
    return false;
}

std::vector<int> EucGraph::getAdjlist(int k){
    assert(k < this->n_vertices);
    return this->edges[k];
}

std::vector<std::vector<int> > EucGraph::getAdjlist(){
    return this->edges;
}

std::vector<Point> EucGraph::getVertices(){
    return this->vertices;
}
