#pragma once
#include "DCEL/DCEL.h"
#include "DCEL/Polygon.h"
#include "C_Subdivision.h"
#include <string>
#include <unordered_map>
#include <vector>

class CS_Free{
protected:
    enum Vertex_Type{V_SRC, V_OBS, V_TRP};   //source vertex, obstacle vertex, transparent vertex
    enum HEdge_Type{HE_TRP, HE_OPQ};   //transparent edge, opaque edge
    std::unordered_map<std::string, Vertex_Type> vertices_types;      //vertex key, vertex type
    std::unordered_map<std::string, HEdge_Type> edge_types;           //HEdge key, HEdge type
    DCEL* subdiv;
public:
    CS_Free(Point src, std::vector<SimplePolygon>& obstacles);
    ~CS_Free();
};