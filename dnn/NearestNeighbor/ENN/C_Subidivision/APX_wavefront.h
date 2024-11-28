#pragma once


#include <vector>
#include "DCEL/DCEL.h"

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class WF_generator {
public:
    Vertex* src;  // Source vertex from which the wavefront is generated
    HEdge* hedge; // each marked generator is in the approximate wavefront of one of the boundary edges of c(cell)
    double weight; // distance from src to hedge 
    bool operator==(WF_generator);
};

//An approximate wavefront represents the wavefront reaching an edge from one side of the edge only.
class APX_wavefront{ 
public:
    Direction reach_from;
    std::vector<WF_generator> wavefront; 
    APX_wavefront();
    ~APX_wavefront();

    std::vector<WF_generator> get_generators();
    std::pair<WF_generator, WF_generator> compute_claim_range(HEdge* e, int idx); 
    std::pair<WF_generator, WF_generator> compute_claim_range(HEdge* e, WF_generator & g); 
};


