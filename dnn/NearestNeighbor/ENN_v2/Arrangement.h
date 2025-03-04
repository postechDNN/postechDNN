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
    Arrangement() :DCEL() {};
    Arrangement(const vector<Point>&);
};