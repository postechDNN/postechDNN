#include "CS_Free.h"

DCEL constructObsDCEL(std::vector<SimplePolygon>& obstacles, std::string key = "__default__");

CS_Free::CS_Free(Point src, std::vector<SimplePolygon>& obstacles){
    //1. Collect all points of obstacles and source.
    std::vector<Point> pts={src};
    for(auto obs: obstacles){
        auto obs_vertices = obs.getVertices();
        pts.insert(pts.end(),obs_vertices.begin(), obs_vertices.end());
    }

    //2. Make a strong 2-conforming subdivision for points.
    C_Subdivision CS(pts);
    DCEL S = CS.build_alpha_subdivision(2);

    //3. Construct DCEL for obstacles and label vertices and half edges of the DCEL.
    DCEL D_obs = constructObsDCEL(obstacles);
    
    //4. Merge the conforming subdivision and the DCEL for obstacles into S_overlay.
    DCEL S_overlay = S.merge(D_obs);

    //5. Store vertices type and half edge types by using labels of S_overlay. 
    {
        std::vector<HEdge*> edges = S_overlay.getHedges();
        std::vector<Vertex*> vertices = S_overlay.getVertices();
        for (auto e : edges) {
            std::string key = e->getKey();
            if (key.front() == "_") {
                this->edge_types[key] = TRP;
            }
            else {
                this->edge_types[key] = OPQ;
            }
        }
        for (auto v : vertices) {
            std::string key = v->getKey();
            Point pos = v->getPoint();
            //src
            if (std::abs(src.getx() - pos.getx()) < 1e-6 && std::abs(src.gety() - pos.gety()) < 1e-6) {
                this->vertices_types[key] = SRC;
            }
            else if (key.front() == "_") {
                this->vertices_types[key] = OBS;
            }
            else {
                this->vertices_types[key] = TRP;
            }
        }
    }
    //6. Keep intact the cells in S_overaly which is incident to an obstacle vertex or S, 
    //  and delete every edge fragment of S not on the boundary of one of these interesting cells.
    // HOW: Uninteresting cell 구하고, interesting cell의 half edge만 key를 모아두고


    //7. Partition each cell containing an obstacle vertex v by extending edges vertically up and down from v.
    // HOW: interesting cell일 것이고, 새로운 vertex, half edge 추가, graph로 만들어서 이를 다시 DCEL로.

}

CS_Free::~CS_Free(){}

DCEL constructObsDCEL(std::vector<SimplePolygon>& obstacles, std::string key) {
    //std::vector<Point>&, std::vector<std::vector<int>>&
    std::vector<Point> vertex;
    std::vector<std::vector<int>> graph;

    for (auto nowPolygon : obstacles) {
        int startIdx = vertex.size();
        std::vector<Point> nowPts = nowPolygon.getVertices();
        for (auto p : nowPts) point.push_back(p);
        for (int i = 0; i < nowPts.size() - 1; i++) {
            std::vector<int> edge;
            edge.push_back(startIdx + i);
            edge.push_back(startIdx + i + 1);
            graph.push_back(edge);
        }
        std::vector<int> edge;
        edge.push_back(startIdx + nowPts.size() - 1);
        edge.push_back(startIdx);
        graph.push_back(edge);
    }

    DCEL ret(point, graph, key);
    return ret;
}