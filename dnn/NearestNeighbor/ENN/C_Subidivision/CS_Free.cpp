#include "CS_Free.h"
#include "PointLocation/Location.h"

struct PLQ {
    HEdge* top;
    HEdge* bottom;
    HEdge* right;
    HEdge* left;
};

DCEL constructObsDCEL(std::vector<SimplePolygon>& obstacles, std::string key = "__default__");

void point_location_queries(std::vector<Vertex*> *s_vertices, DCEL *obstacles, std::vector<PLQ> &results);

constexpr double TOLERANCE_CS_FREE = 1e-6;
Point upperRayIntersection(Point origin, Edge edge);
Point lowerRayIntersection(Point origin, Edge edge);
bool isEdgeVertical(const HEdge* const edge);
bool isEdgeHorizontal(const HEdge* const edge);

CS_Free::CS_Free(Point src, std::vector<SimplePolygon>& obstacles, bool nlogn){
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
    
    DCEL S_overlay;

    if (nlogn) {
        // 4. Make S'(S_overay) using point location
        std::vector<Vertex*> s_vertices = S.getVertex();
        std::vector<PLQ> plq;
        // std::vector<PLQ> point_location_query(s_vertices, PLQ);
        // void point_location_queries(&s_vertices, &D_obs, point_location_query);
        Location loc(&D_obs);

        for (auto v : s_vertices) {
            PLQ tmp;
            tmp.top = loc.ortho_ray(&(v->getPoint()), Location::N);
            tmp.bottom = loc.ortho_ray(&(v->getPoint()), Location::S);
            tmp.left = loc.ortho_ray(&(v->getPoint()), Location::W);
            tmp.right = loc.ortho_ray(&(v->getPoint()), Location::E);
            plq.push_back(tmp);
        }

        // 5. Store vertices type and half edge types by using labels of S_overlay.

        //6. Keep intact the cells in S_overaly which is incident to an obstacle vertex or S, 
        //  and delete every edge fragment of S not on the boundary of one of these interesting cells.
        // HOW: Uninteresting cell 구하고, interesting cell의 half edge만 key를 모아두고
    }

    else {
        //4. Merge the conforming subdivision and the DCEL for obstacles into S_overlay.
        S_overlay = S.merge(D_obs);

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
        {
            std::vector<Face*> faces = S_overlay.getFaces();
            std::vector<Face*> uninterFace;

            // Find uninteresting cell
            for (auto face : faces) {
                // For each vertex of S_overaly, determine whether the vertex is a vertex of 'S' or 'O'
                std::vector<Vertex*> vertices = face.getInnerVertices();
                for (auto vertex : vertices) {
                    std::string key = vertex->getKey();
                    if (key.empty()) continue;
                    // the vertex is a vertex of 'S'
                    if (key.back() == '_') {
                        uninterFace.push_back(face);
                        break;
                    }
                    // the vertex is a vertex of 'O'
                    if (key.front() == '_') {
                        uninterFace.push_back(face);
                        break;
                    }
                }
            }

            // Delete every edge fragment
            for (auto face : uninterFace) {
                // For every pair of incident edges(a,b), determine wheter the edges are fragment edges.
                std::vector<HEdge*> edges = face->getInnerHEdges();
                if (edges.empty()) continue;
                Point startV = edges[0]->getEdge().gets();
                HEdge* nowE = edges[0];
                while (nowE->getNext()->getEdge().gets() != startV) {
                    // if a is horizontal
                    if (isEdgeHorizontal(nowE)) {
                        if (isEdgeHorizontal(nowE->getNext())) {
                            // Merge
                        }
                    }
                    // if a is vertical
                    else if (isEdgeVertical(nowE)) {
                        if (isEdgeVertical(nowE->getNext())) {
                            // Merge
                        }
                    }
                }
            }
        }
    }
    
    
    //7. Partition each cell containing an obstacle vertex v by extending edges vertically up and down from v.
    // HOW: interesting cell일 것이고, 새로운 vertex, half edge 추가, graph로 만들어서 이를 다시 DCEL로.
    {
        std::vector<Face*> added_faces;
        std::vector<Face*> faces = S_overlay.getFaces();
        std::vector<double> deltas;
        // Compute delta
        for (auto f : faces) {
            std::vector<HEdge*> edges = f->getInnerHEdges();
            double delta = DBL_MAX;
            for (auto he : edges) {
                if (this->vertices_types[he->getOrigin()->getKey()] == TRP&&
                    this->vertices_types[he->getNext()->getOrigin()->getKey()] == TRP) {
                    double temp = he->getOrigin()->getPoint().distance(he->getNext()->getOrigin()->getPoint());
                    if (delta > temp) delta = temp;
                }
            }
            deltas.push_back(delta);
        }
        
        for (size_t i = 0; i < faces.size(); i++) {
            auto f = faces[i];
            std::vector<Vertex*> vertices = f->getInnerVertices();
            for (auto v : vertices) {
                if (this->vertices_types[v->getKey()] == OBS) {
                    std::vector<HEdge*> edges = f->getInnerHEdges();
                    HEdge* het;
                    HEdge* heb;
                    for (auto he : edges) {
                        Edge e = he->getEdge();
                        Point es = e.gets();
                        Point et = e.gett();
                        // 예외처리 (v 포함하는 edge일 경우)
                        if (std::abs(es.getx() - v->getx()) < TOLERANCE_CS_FREE &&
                            std::abs(es.gety() - v->gety()) < TOLERANCE_CS_FREE) {
                            continue;
                        }
                        if (std::abs(et.getx() - v->getx()) < TOLERANCE_CS_FREE &&
                            std::abs(et.gety() - v->gety()) < TOLERANCE_CS_FREE) {
                            continue;
                        }

                        double x[2];
                        if (es.getx() > et.getx()) {
                            x[0] = et.getx(); x[1] = es.getx();
                        }
                        else {
                            x[0] = es.getx(); x[1] = et.getx();
                        }

                        double y[2];
                        if (es.gety() > et.gety()) {
                            y[0] = et.gety(); y[1] = es.gety();
                        }
                        else {
                            y[0] = es.gety(); y[1] = et.gety();
                        }


                        if (x[0] < v->getx() && v->getx() < x[1]) {
                            // v 위에 있는 edge 찾기 (부등호 예외 처리 확인)
                            if (v->gety() < y[1]) {
                                het = he;
                            }
                            // v 아래 있는 edge 찾기
                            else if (y[1] < v->gety()) {
                                heb = he;
                            }
                        }
                    }

                    // Original cell update


                    // Obstacle vertex 기준 Upper face 만들기 
                    Face* uf = new Face();

                    // Obstacle vertex 기준 Lower face 만들기
                    Face* lf = new Face();

                    // Cell 하나에 obstacle vertex 하나만 있므로 break
                    
                    break;
                }
            }
        }

        for (auto af : added_faces) {
            faces.push_back(af);
        }

        // Construct graph for result DCEL
        std::vector<Point> G_V;
        std::unordered_map<std::string, int> G_V_map; // vertex key, index of G_V
        std::vector<std::vector<int>> G_E;
        std::vector<int> temp_list;
        for (auto f : faces) {
            std::vector<HEdge*> hedges = f->getInnerHEdges();
            for (auto he : hedges) {
                Vertex u = he->getOrigin();
                Vertex v = he->getNext()->getOrigin();
                if (G_V_map.find(u.getKey()) == m.end()) {
                    G_V_map[u.getKey()] = G_V.size();
                    G_V.push_back(u.getPoint());
                    G_E.push_back(temp_list);
                }
                if (G_V_map.find(v.getKey()) == m.end()) {
                    G_V_map[v.getKey()] = G_V.size();
                    G_V.push_back(v.getPoint());
                    G_E.push_back(temp_list);
                }
                G_E[G_V_map[u.getKey()]].push_back(G_V_map[v.getKey()]);
            }
        }

        this->subdiv = DCEL(G_V,G_E);
    }
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

Point upperRayIntersection(Point origin, Edge edge) {
    Point result;

    return result;
}

Point lowerRayIntersection(Point origin, Edge edge) {
    Point result;

    return result;
}

bool isEdgeVertical(const HEdge* const edge) {
    return (std::abs(edge->getEdge().gets().gety() - edge->getEdge().gett().gety()) < TOLERANCE_CS_FREE);
}

bool isEdgeHorizontal(const HEdge* const edge) {
    return (std::abs(edge->getEdge().gets().getx() - edge->getEdge().gett().getx()) < TOLERANCE_CS_FREE);
}

void point_location_queries(std::vector<Vertex*>* s_vertices, DCEL* obstacles, std::vector<PLQ>& results) {

}