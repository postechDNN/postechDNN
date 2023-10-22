#include "CS_Free.h"

struct PLQ {
    HEdge* top;
    HEdge* bottom;
    HEdge* right;
    HEdge* left;
};

DCEL constructObsDCEL(std::vector<SimplePolygon>& obstacles, std::string key = "__default__");

void point_location_queries(std::vector<Vertex*> *s_vertices, DCEL *obstacles, std::vector<PLQ> &results);

void make_Hedge_list(HEdge* start, std::vector<HEdge*> &result);

constexpr double TOLERANCE_CS_FREE = 1e-6;
Point verticalRayIntersection(Point origin, Edge edge);
Point horizontalRayIntersection(Point origin, Edge edge);
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
        std::vector<PLQ> point_location_query(s_vertices, PLQ);
        void point_location_queries(&s_vertices, &D_obs, point_location_query);

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
                    // obstacle vertex 기준 잘라야할 edge 찾기 (위,아래)
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

                    // Make new edges
                    std::vector<HEdge*> upperLeft, upperRight, lowerLeft, lowerRight; // index가 작을수록 obstacle vertex와 가까움

                    Point topIntersectionPoint, bottomIntersectionPoint;
                    topIntersectionPoint = verticalRayIntersection(v->getPoint(), het->getEdge());
                    bottomIntersectionPoint = verticalRayIntersection(v->getPoint(), het->getEdge());

                    int upperEdgeNumber, lowerEdgeNumber;                    
                    upperEdgeNumber = std::floor(std::abs(v->getPoint().gety() - topIntersectionPoint.gety()) / deltas[i]);
                    lowerEdgeNumber = std::floor(std::abs(v->getPoint().gety() - bottomIntersectionPoint.gety()) / deltas[i]);
                    
                    double upperEdgeLength, lowerEdgeLength;
                    upperEdgeLength = std::abs(v->getPoint().gety() - topIntersectionPoint.gety()) / double(upperEdgeNumber);
                    lowerEdgeLength = std::abs(v->getPoint().gety() - bottomIntersectionPoint.gety()) / double(lowerEdgeNumber);

                    std::vector<Vertex*> upperVertices, lowerVertices;
                    upperVertices.push_back(v);
                    lowerVertices.push_back(v);

                    // Make upper hedges and Set Twin
                    for (int i = 0; i < upperEdgeNumber; i++) {
                        Vertex* newV = new Vertex(v->getx(), v->gety() + upperEdgeLength);
                        upperVertices.push_back(newV);
                        HEdge* leftHE = new HEdge();
                        HEdge* rightHE = new HEdge();
                        leftHE->setOrigin(upperVertices[i]);
                        rightHE->setOrigin(upperVertices[i + 1]);
                        leftHE->setTwin(rightHE);
                        rightHE->setTwin(leftHE);
                        upperLeft.push_back(leftHE);
                        upperRight.push_back(rightHE);
                    }

                    // Set Next and Prev
                    for (int i = 0; i < upperEdgeNumber - 1; i++) {
                        upperLeft[i]->setNext(upperLeft[i+1]);
                        upperLeft[i + 1]->setPrev(upperLeft[i]);
                        upperRight[i + 1]->setNext(upperRight[i]);
                        upperRight[i]->setPrev(upperRight[i + 1]);
                    }

                    // Make lower hedges and Set twin
                    for (int i = 0; i < lowerEdgeNumber; i++) {
                        Vertex* newV = new Vertex(v->getx(), v->gety() - lowerEdgeLength);
                        lowerVertices.push_back(newV);
                        HEdge* leftHE = new HEdge();
                        HEdge* rightHE = new HEdge();
                        leftHE->setOrigin(lowerVertices[i + 1]);
                        rightHE->setOrigin(lowerVertices[i]);
                        leftHE->setTwin(rightHE);
                        rightHE->setTwin(leftHE);
                        lowerLeft.push_back(leftHE);
                        lowerRight.push_back(rightHE);
                    }

                    // Set Next and Prev
                    for (int i = 0; i < lowerEdgeNumber - 1; i++) {
                        lowerLeft[i + 1]->setNext(lowerLeft[i]);
                        lowerLeft[i]->setPrev(lowerLeft[i + 1]);
                        lowerRight[i]->setNext(lowerRight[i + 1]);
                        lowerRight[i + 1]->setPrev(lowerRight[i]);
                    }


                    std::vector<HEdge*> edges = f->getInnerHEdges();
                    HEdge* firstEdge; // whose origin is obstacle vertex
                    for (auto he : edges) {
                        Vertex nowV = he->getOrigin();
                        if (std::abs(v->getx() - nowV.getx()) < TOLERANCE_CS_FREE && std::abs(v->gety() - nowV.gety()) < TOLERANCE_CS_FREE) {
                            firstEdge = he;
                            break;
                        }
                    }

                    bool isUpper = false; // If Hedge whose origin is obstacle vertex is included in new upper cell then true
                    HEdge* upFaceFirst, *loFaceFirst, *upFaceLast, *loFaceLast;
                    HEdge* originFaceFirst, *originFaceLast;
                    HEdge* nowHEdge = firstEdge;
                    //HEdge* nextEdge = firstEdge->getNext();
                    while (true) {
                        nowHEdge = nowHEdge->getNext();
                        //nextEdge = nowEdge->getNext();
                        Edge nowEdge = nowHEdge->getEdge();
                        Edge et = het->getEdge();
                        Edge eb = heb->getEdge();
                        if (std::abs(nowEdge.gets().getx() - et.gets().getx()) < TOLERANCE_CS_FREE &&
                            std::abs(nowEdge.gets().gety() - et.gets().gety()) < TOLERANCE_CS_FREE &&
                            std::abs(nowEdge.gett().getx() - et.gett().getx()) < TOLERANCE_CS_FREE &&
                            std::abs(nowEdge.gett().gety() - et.gett().gety()) < TOLERANCE_CS_FREE) {
                            isUpper = true;
                            loFaceLast = firstEdge->getPrev();

                            // Split original edges                           
                            upFaceFirst = firstEdge;
                            upFaceLast = nowHEdge;
                            originFaceFirst = new HEdge();
                            originFaceFirst->setOrigin(upperVertices.back());
                            originFaceFirst->setNext(nowHEdge->getNext());
                            originFaceFirst->getNext()->setPrev(originFaceFirst);

                            loFaceFirst = new HEdge();
                            originFaceLast = heb;
                            loFaceFirst->setOrigin(lowerVertices.back());
                            loFaceFirst->setNext(originFaceLast->getNext());
                            loFaceFirst->getNext()->setPrev(loFaceFirst);

                            // Update DCEL of neighbor cell
                            HEdge* upFaceTwin = new HEdge();
                            HEdge* originFaceFirstTwin = nowHEdge->getTwin();
                            upFaceTwin->setOrigin(upperVertices.back());
                            upFaceTwin->setNext(originFaceFirstTwin->getNext());
                            upFaceTwin->getNext()->setPrev(upFaceTwin);
                            upFaceTwin->setPrev(originFaceFirstTwin);
                            upFaceTwin->setTwin(upFaceLast);
                            upFaceLast->setTwin(upFaceTwin);
                            originFaceFirstTwin->setNext(upFaceTwin);
                            originFaceFirstTwin->setTwin(originFaceFirst);
                            originFaceFirst->setTwin(originFaceFirstTwin);

                            HEdge* loFaceTwin = heb->getTwin();
                            HEdge* originFaceLastTwin = new HEdge();
                            originFaceLastTwin->setOrigin(lowerVertices.back());
                            originFaceLastTwin->setNext(loFaceTwin->getNext());
                            originFaceLastTwin->getNext()->setPrev(originFaceLastTwin);
                            originFaceLastTwin->setPrev(loFaceTwin);
                            originFaceLastTwin->setTwin(originFaceLast);
                            originFaceLast->setTwin(originFaceLastTwin);
                            loFaceTwin->setNext(originFaceLastTwin);
                            loFaceTwin->setTwin(loFaceFirst);
                            loFaceFirst->setTwin(loFaceTwin);
                            break;
                        }
                        else if (std::abs(nowEdge.gets().getx() - eb.gets().getx()) < TOLERANCE_CS_FREE &&
                            std::abs(nowEdge.gets().gety() - eb.gets().gety()) < TOLERANCE_CS_FREE &&
                            std::abs(nowEdge.gett().getx() - eb.gett().getx()) < TOLERANCE_CS_FREE &&
                            std::abs(nowEdge.gett().gety() - eb.gett().gety()) < TOLERANCE_CS_FREE) {
                            upFaceLast = firstEdge->getPrev();

                            // Split original edges
                            loFaceFirst = firstEdge;
                            loFaceLast = nowHEdge;
                            originFaceFirst = new HEdge();
                            originFaceFirst->setOrigin(lowerVertices.back());
                            originFaceFirst->setNext(nowHEdge->getNext());
                            originFaceFirst->getNext()->setPrev(originFaceFirst);

                            upFaceFirst = new HEdge();
                            originFaceLast = het;
                            upFaceFirst->setOrigin(upperVertices.back());
                            upFaceFirst->setNext(originFaceLast->getNext());
                            upFaceFirst->getNext()->setPrev(upFaceFirst);

                            // Update DCEL of neighbor cell
                            HEdge* loFaceTwin = new HEdge();
                            HEdge* originFaceFirstTwin = nowHEdge->getTwin();
                            loFaceTwin->setOrigin(lowerVertices.back());
                            loFaceTwin->setNext(originFaceFirstTwin->getNext());
                            loFaceTwin->getNext()->setPrev(loFaceTwin);
                            loFaceTwin->setPrev(originFaceFirstTwin);
                            loFaceTwin->setTwin(loFaceLast);
                            loFaceLast->setTwin(loFaceTwin);
                            originFaceFirstTwin->setNext(loFaceTwin);
                            originFaceFirstTwin->setTwin(originFaceFirst);
                            originFaceFirst->setTwin(originFaceFirstTwin);

                            HEdge* upFaceTwin = het->getTwin();
                            HEdge* originFaceLastTwin = new HEdge();
                            originFaceLastTwin->setOrigin(upperVertices.back());
                            originFaceLastTwin->setNext(upFaceTwin->getNext());
                            originFaceLastTwin->getNext()->setPrev(originFaceLastTwin);
                            originFaceLastTwin->setPrev(upFaceTwin);
                            originFaceLastTwin->setTwin(originFaceLast);
                            originFaceLast->setTwin(originFaceLastTwin);
                            upFaceTwin->setNext(originFaceLastTwin);
                            upFaceTwin->setTwin(upFaceFirst);
                            upFaceFirst->setTwin(upFaceTwin);

                            break;
                        }
                    }

                    // Original cell update
                    if (isUpper) {
                        upperLeft.back()->setNext(originFaceFirst);
                        lowerLeft.front()->setNext(upperLeft.front());
                        originFaceLast.setNext(lowerLeft.back());
                    }
                    else {
                        lowerRight.back()->setNext(originFaceFirst);
                        upperRight.front()->setNext(lowerRight.front());
                        originFaceLast.setNext(upperRight.back());
                    }
                    std::vector<HEdge*> fHedgeList;
                    f->setInners(&fHedgeList);

                    // Obstacle vertex 기준 Upper face 만들기 
                    Face* uf = new Face();
                    std::vector<HEdge*> ufHedgeList;
                    if (isUpper) {
                        upperRight.front()->setNext(upFaceFirst);
                        upFaceLast.setNext(upperRight.back());
                    }
                    else {
                        upperLeft.back()->setNext(upFaceFirst);
                        upFaceLast.setNext(upperLeft.front());
                    }
                    make_Hedge_list(upFaceFirst, &ufHedgeList);
                    uf->setInners(&ufHedgeList);
                    

                    // Obstacle vertex 기준 Lower face 만들기
                    Face* lf = new Face();
                    std::vector<HEdge*> lfHedgeList;
                    if (isUpper) {
                        lowerRight.back()->setNext(loFaceFirst);
                        loFaceLast.setNext(upperRight.front());
                    }
                    else {
                        lowerLeft.front()->setNext(loFaceFirst);
                        loFaceLast.setNext(upperLeft.back());
                    }
                    make_Hedge_list(upFaceFirst, &ufHedgeList);
                    lf->setInners(&lfHedgeList);

                    added_faces.push_back(uf);
                    added_faces.push_back(lf);

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
        // Update edge and vertical map
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

Point verticalRayIntersection(Point origin, Edge edge) {
    Point result;
    result.setx(origin.getx());
    double slope;
    double intercept;
    Point s = edge.gets();
    Point t = edge.gett();

    // If edge if horizontal

    slope = (s.gety() - t.gety()) / (s.getx() - t.getx());
    intercept = s.gety() - slope * s.getx();

    result.sety(slope*result.getx() +intercept);

    return result;
}

Point horizontalRayIntersection(Point origin, Edge edge) {
    Point result;
    result.sety(origin.gety());
    double slope;
    double intercept;
    Point s = edge.gets();
    Point t = edge.gett();

    // If edge is vertical
    if (std::abs(s.getx()-t.getx()) < TOLERANCE_CS_FREE) {
        result.setx(s.getx());
    }

    else {
        slope = (s.gety() - t.gety()) / (s.getx() - t.getx());
        intercept = s.gety() - slope * s.getx();

        result.setx((result.gety() - intercept) / slope);
    }
    

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

void make_Hedge_list(HEdge* start, std::vector<HEdge*>& result) {
    Point s = start->getOrigin()->getPoint();
    
    HEdge* nowEdge = start;
    while (true) {
        result.push_back(nowEdge);
        nowEdge = nowEdge->getNext();
        Point nowP = nowEdge->getOrigin()->getPoint();
        if (std::abs(s.getx() - nowP.getx()) < TOLERANCE_CS_FREE && std::abs(s.gety() - nowP.gety()) < TOLERANCE_CS_FREE) {
            break;
        }
    }
}