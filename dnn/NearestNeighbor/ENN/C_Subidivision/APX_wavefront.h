#pragma once


#include <vector>
#include "DCEL/DCEL.h"

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

//An approximate wavefront represents the wavefront reaching an edge from one side of the edge only.
class APX_wavefront{ 
public:
    Direction reach_from;     
    APX_wavefront();
    ~APX_wavefront();

    std::vector<Point> get_generators();
    std::pair<Point, Point> compute_claim_range(HEdge* e, int idx); 
    std::pair<Point, Point> compute_claim_range(HEdge* e, Point & pt); 
};


std::vector<Point> APX_wavefront::get_generators(){
    
}

std::pair<Point, Point> APX_wavefront::compute_claim_range(HEdge* e, int idx){
    //
}

std::pair<Point, Point> APX_wavefront::compute_claim_range(HEdge* e, Point & pt){

}
