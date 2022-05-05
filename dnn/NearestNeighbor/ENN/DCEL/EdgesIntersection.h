#pragma once
#include "AVLTree.h"
#include <iostream>
#include <string>
#include "Edge.h"
#include "Point.h"
#include <queue>
#include <set>
#include <cmath>
#include "EucGraph.h"
#include <utility>

#define tolerance 1e-6 //TODO: change to enum
#define __DEBUG_MODE__ 0

class EdgeContainer{
public:
    int key;
    std::vector<Edge> *edges;
    Point *sweep_p;
    EdgeContainer();
    EdgeContainer(int k,std::vector<Edge>* edges, Point *sweep_p);
    EdgeContainer(const EdgeContainer& op);
    double getKey() const;
    bool operator <(const EdgeContainer& op) const;
    bool operator==(const EdgeContainer& op) const;
};

class intersectEvent{
public:
    enum class EVENT{START =0 , END, CROSS};
    Point p;
    int key;
    EVENT ev;
    intersectEvent(const Point& p, int key ,EVENT ev);
    bool operator<(const intersectEvent op) const;
    bool operator==(intersectEvent& op);
};

class EdgesIntersection{ //output 고민좀 해보자
private: 
    Point sweep_p;
    AVLTree<EdgeContainer> T;
    std::priority_queue<intersectEvent> pq; 
    std::vector<Edge> edges;
    EucGraph graph;
    void FindNewEvent(EdgeContainer ec_1, EdgeContainer ec_2);
public: 
    EdgesIntersection(std::vector<Edge> edges);
    EucGraph getGraph();
};

