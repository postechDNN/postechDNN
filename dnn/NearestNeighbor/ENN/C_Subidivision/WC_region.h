#pragma once

#include "DCEL/DCEL.h"

class WC_region{
public:
    std::vector<Face*> regions;
    std::vector<HEdge*> boundary;
    HEdge* e;

    WC_region(DCEL&, HEdge* he);
};
