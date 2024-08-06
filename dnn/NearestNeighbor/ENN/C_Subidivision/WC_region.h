#pragma once

#include "DCEL/DCEL.h"
#include "CS_Free.h"

class WC_region{
public:
    std::vector<Face*> regions;
    std::vector<HEdge*> boundary;
    HEdge* e;

    WC_region(DCEL&, HEdge* he);
};

class WC_region_free{
public:
    std::vector<Face*> regions;
    std::vector<HEdge*> boundary;
    HEdge* e;

    WC_region_free(CS_Free&, HEdge* he);
};
