#pragma once

#include <cassert>
#include <vector>

template <typename T>
class Graph{ //undirected graph
private:
    int n_vertices;
    int n_edges;
    std::vector<T> vertices;
    std::vector<std::vector<int> > edges;
public:
    Graph();
    ~Graph();
    int size_vertices();
    int size_edges();
    int insert_vertex(T); //O(1)
    void insert_edge(int, int); //O(1)
    T getVertex(int); //O(1)
    bool is_connected(int,int); //O(E)
    std::vector<int> getAdjlist(int); //O(E_v) where E_v is the number of edges adjacent to the vertex 
    std::vector<std::vector<int> > getAdjlist();
    std::vector<T> getVertices(); // O(V) where V is the number of vertices
};

template <typename T>
Graph<T>::Graph(){
    this->n_edges = this->n_vertices = 0;
}

template <typename T>
Graph<T>::~Graph(){}

template <typename T>
int Graph<T>::size_vertices(){
    return this->n_vertices;
}

template <typename T>
int Graph<T>::size_edges(){
    return this->n_edges;
}

template <typename T>
int Graph<T>::insert_vertex(T p){
    int key = this->n_vertices;
    this->n_vertices++;
    this->vertices.push_back(p); 
    this->edges.push_back(std::vector<int>());
    return key;
}

template <typename T>
void Graph<T>::insert_edge(int k1, int k2){
    assert(k1 < this->n_vertices && k2 < this->n_vertices);
    this->n_edges++;
    this->edges[k1].push_back(k2);
    this->edges[k2].push_back(k1);
}

template <typename T>
T Graph<T>::getVertex(int k){
    assert(k < this->n_vertices);
    return this->vertices[k];
}

template <typename T>
bool Graph<T>::is_connected(int k1,int k2){
    assert(k1 < this->n_vertices && k2 < this->n_vertices);
    for(auto k:edges[k1]){
        if(k == k2) return true;
    }
    return false;
}

template <typename T>
std::vector<int> Graph<T>::getAdjlist(int k){
    assert(k < this->n_vertices);
    return this->edges[k];
}

template <typename T>
std::vector<std::vector<int> > Graph<T>::getAdjlist(){
    return this->edges;
}

template <typename T>
std::vector<T> Graph<T>::getVertices(){
    return this->vertices;
}
