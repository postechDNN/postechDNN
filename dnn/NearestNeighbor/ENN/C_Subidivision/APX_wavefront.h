#pragma once

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
};