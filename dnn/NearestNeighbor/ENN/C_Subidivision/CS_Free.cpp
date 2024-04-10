#include "CS_Free.h"
#include "PointLocation/Location.h"

#include <algorithm>
#include <cfloat>
#include <fstream>
#include <numbers>
#include <unordered_set>

constexpr double TOLERANCE_CS_FREE_TEMP = 1e-4;
constexpr double TOLERANCE_CS_FREE = 1e-6;
constexpr double PI_CS_FREE = 3.141592653589793238462643383279502884L;

// For each vertex, store results of ray shooting query
struct PLQ {
    HEdge* answer[4] = { NULL, NULL, NULL, NULL }; // top, bottom, right, left
};

// For each vertex, store adjacent (neighbor) vertices
struct Neighbor {
    bool isNull[4] = { true, true, true, true };
    bool isChanged[4] = { false, false, false, false };
    Point neighbor[4];
    int v_idx[4] = { -1, -1, -1, -1 };
    int new_v_idx[4] = { -1, -1, -1, -1 };
};

// 
DCEL* constructObsDCEL(std::vector<SimplePolygon>& obstacles, std::string key = "__default__");

// For each vertex of s_vertices, store results of ray shooting query in PLQ
void point_location_queries(std::vector<Vertex*> *s_vertices, DCEL *obstacles, std::vector<PLQ> &results);

// Store connected HEdge component in result
void make_Hedge_list(HEdge* start, std::vector<HEdge*> &result);

// Return intersection point between edge and vertical line including origin
Point verticalRayIntersection(Point origin, Edge edge);
// Return intersection point between edge and horizontal line including origin
Point horizontalRayIntersection(Point origin, Edge edge);
// Return true if edge is vertical line segment
bool isEdgeVertical(HEdge* const edge);
// Return true if edge is horizontal line segment
bool isEdgeHorizontal(HEdge* const edge);
// To sort point ascending y-coordinate
bool comparePoint1(std::pair<Point, int> a, std::pair<Point, int> b);
// To sort point descending y-coordinate
bool comparePoint2(std::pair<Point, int> a, std::pair<Point, int> b);
// To sort point ascending x-coordinate
bool comparePoint3(std::pair<Point, int> a, std::pair<Point, int> b);
// To sort point descending x-coordinate
bool comparePoint4(std::pair<Point, int> a, std::pair<Point, int> b);

// Returns true when p lying beside two edges
bool isPointInner(Point p, HEdge* prev, HEdge* now);
// Returns CCW angle abc.
double getAngle(Point a, Point b, Point c);
// -1 CW, 0 Colinear, 1 CCW
int getDirection(Point a, Point b, Point c);

// To print dcel for debugging
void print_result(const std::vector<Point>* const G_V, const std::unordered_map<std::string, int>* const G_V_map, const std::vector<std::vector<int>>* const G_E, const std::string filename);
void print_result(const std::vector<Point>* const G_V, const std::vector<std::vector<int>>* const G_E, const std::string filename);
void print_Neighbor(const Neighbor);
bool sort_by_idx(std::pair<Point,int> a , std::pair<Point, int> b);


CS_Free::CS_Free(Point src, std::vector<SimplePolygon>& obstacles){
    //1. Collect all points of obstacles and source.
    std::vector<Point> pts={src};
    for(auto obs: obstacles){
        auto obs_vertices = obs.getVertices();
        pts.insert(pts.end(),obs_vertices.begin(), obs_vertices.end());
    }
    std::cout << "checkpoint1\n";

    //2. Make a strong 2-conforming subdivision for points.
    C_Subdivision CS(pts);
    DCEL S = CS.build_alpha_subdivision(2);
    std::cout << "checkpoint2\n";
    /*
    std::cout << S.getFaces().size() << std::endl;
    std::vector<Face*> debugFace = S.getFaces();
    int debugInners = 0;
    for (size_t i = 0; i < debugFace.size(); i++) {
        if (debugFace[i]->getInnerHEdges().size() != 0) std::cout << i << std::endl;
        debugInners += debugFace[i]->getInnerHEdges().size();
    }
    std::cout << debugInners << std::endl;
    */
    //3. Construct DCEL for obstacles and label vertices and half edges of the DCEL.
    DCEL* D_obs = constructObsDCEL(obstacles);

    /*
    std::vector<string> key_list({ "e4_1", "e4_2", "e6_1", "e6_2", "e12_1", "e12_2", "e14_1", "e14_2" });
    std::vector<CEdge> edge_list;
    for (string s : key_list) {
        edge_list.push_back(CEdge(*(D_obs->getHedge(s))));
    }
    std::cout << "edge_list size: " << edge_list.size() << '\n';
    for (CEdge x : edge_list) {
        std::cout << x.getKey() << " (" << x.getOrigin()->getx() << ", " << x.getOrigin()->gety() << ")-("
            << x.getNext()->getOrigin()->getx() << ", " << x.getNext()->getOrigin()->gety() << ")\n";
    }
    CEdge::setConstantLine(5.0, false);
    sort(edge_list.begin(), edge_list.end());
    std::cout << "sorted: \n";
    for (CEdge x : edge_list) {
        std::cout << x.getKey() << " (" << x.getOrigin()->getx() << ", " << x.getOrigin()->gety() << ")-("
            << x.getNext()->getOrigin()->getx() << ", " << x.getNext()->getOrigin()->gety() << ")\n";
    }

    return;
    */

    std::cout << "checkpoint3\n";

    // point location debug
    /*
    std::vector<Vertex*> s_vertices_debug;
    Vertex* debug_vertex = new Vertex();
    debug_vertex->setx(5.0);
    debug_vertex->sety(1.125);
    s_vertices_debug.push_back(debug_vertex);
    std::vector<PLQ> point_location_query_debug(s_vertices_debug.size(), PLQ());
    point_location_queries(&s_vertices_debug, D_obs, point_location_query_debug);
    for (int i = 0; i < 4; i++) {
        if (point_location_query_debug[0].answer[i] != NULL) {
            HEdge* nowEdge = point_location_query_debug[0].answer[i];
            std::cout << i << ' ' << nowEdge->getKey() << " (" << nowEdge->getOrigin()->getx() << ", " << nowEdge->getOrigin()->gety() << ")-("
                << nowEdge->getNext()->getOrigin()->getx() << ", " << nowEdge->getNext()->getOrigin()->gety() << ")\n";
        }
    }
    */

    // debug
    /*
    std::vector<Face*> debugFaces = D_obs.getFaces();
    for (auto f : debugFaces) {
        std::vector<HEdge*> debugHedge = f->getInnerHEdges();
        for (auto hedge : debugHedge) {
            std::cout << hedge->getKey() << ' ' << hedge->getOrigin()->getx() << ' ' << hedge->getOrigin()->gety() << std::endl;
        }
    }
    */

    // 4. Make S'(S_overay) using point location
    std::vector<Vertex*> s_vertices = S.getVertices();
    std::vector<PLQ> point_location_query(s_vertices.size(), PLQ());
    point_location_queries(&s_vertices, D_obs, point_location_query);
        
    // Construct graph for result DCEL
    std::vector<Point> G_V;
    std::unordered_map<std::string, int> G_V_map; // vertex key, index of G_V
    int obs_vertex_index = 0; // O_idx
    int new_TRP_vertex_index = 0; // A_idx
    for (size_t i = 0; i < s_vertices.size(); i++) {
        G_V.push_back(s_vertices[i]->getPoint());
        G_V_map[s_vertices[i]->getKey()] = i;
    }
    std::cout << "checkpoint4\n";

    // debug
    /*
    std::vector<Vertex*> s_vertices2 = D_obs.getVertices();
    int debugint = G_V.size();
    for (size_t i = 0; i < s_vertices2.size(); i++) {
        G_V.push_back(s_vertices2[i]->getPoint());
        if (G_V_map.find(s_vertices2[i]->getKey()) != G_V_map.end()) std::cout << "fail" << std::endl;
        G_V_map[s_vertices2[i]->getKey()] = debugint + i;
    }
    

    std::vector<std::vector<int>> DEBUG_G_E(G_V.size(), std::vector<int>());
    std::vector<HEdge*> debug_edges1 = S.getHedges();
    std::vector<HEdge*> debug_edges2 = D_obs.getHedges();
    for (auto he : debug_edges1) {
        DEBUG_G_E[G_V_map[he->getOrigin()->getKey()]].push_back(G_V_map[he->getNext()->getOrigin()->getKey()]);
    }
    for (auto he : debug_edges2) {
        DEBUG_G_E[G_V_map[he->getOrigin()->getKey()]].push_back(G_V_map[he->getNext()->getOrigin()->getKey()]);
    }

    print_result(&G_V, &DEBUG_G_E, "CS_Free.txt");
    */
    // debug

    // 5. Keep intact the cells in S_overaly which is incident to an obstacle vertex or S,
    std::vector<Neighbor> V_S_Data(s_vertices.size(), Neighbor());
    std::vector<HEdge*> s_edges = S.getHedges();
    int errcnt = 0;
    for (auto hedge : s_edges) {
        Vertex u = *hedge->getOrigin();
        Vertex v = *hedge->getNext()->getOrigin();
        int u_idx = G_V_map[u.getKey()];
        int v_idx = G_V_map[v.getKey()];
        if (std::abs(u.getx() - v.getx()) < TOLERANCE_CS_FREE) {
            if (u.gety() < v.gety()) { // top
                V_S_Data[u_idx].neighbor[0] = v.getPoint();
                V_S_Data[u_idx].isNull[0] = false;
                V_S_Data[u_idx].v_idx[0] = v_idx;
                V_S_Data[v_idx].neighbor[1] = u.getPoint();
                V_S_Data[v_idx].isNull[1] = false;
                V_S_Data[v_idx].v_idx[1] = u_idx;
            }
            else { // bottom
                V_S_Data[u_idx].neighbor[1] = v.getPoint();
                V_S_Data[u_idx].isNull[1] = false;
                V_S_Data[u_idx].v_idx[1] = v_idx;
                V_S_Data[v_idx].neighbor[0] = u.getPoint();
                V_S_Data[v_idx].isNull[0] = false;
                V_S_Data[v_idx].v_idx[0] = u_idx;
            }
        }                       
        else {
            if (u.getx() < v.getx()) { // right
                V_S_Data[u_idx].neighbor[2] = v.getPoint();
                V_S_Data[u_idx].isNull[2] = false;
                V_S_Data[u_idx].v_idx[2] = v_idx;
                V_S_Data[v_idx].neighbor[3] = u.getPoint();
                V_S_Data[v_idx].isNull[3] = false;
                V_S_Data[v_idx].v_idx[3] = u_idx;
            }
            else { // left
                V_S_Data[u_idx].neighbor[3] = v.getPoint();
                V_S_Data[u_idx].isNull[3] = false;
                V_S_Data[u_idx].v_idx[3] = v_idx;
                V_S_Data[v_idx].neighbor[2] = u.getPoint();
                V_S_Data[v_idx].isNull[2] = false;
                V_S_Data[v_idx].v_idx[2] = u_idx;
            }
        }
    }

    // debug
    /*
    std::vector<std::vector<int>> DEBUG_G_E(G_V.size(), std::vector<int>());
    for (int i = 0; i < V_S_Data.size(); i++) {
        for (int j = 0; j < 4; j++) {
            if (V_S_Data[i].isNull[j]) continue;
            DEBUG_G_E[i].push_back(V_S_Data[i].v_idx[j]);
        }
    }
    print_result(&G_V, &DEBUG_G_E, "CS_Free.txt");
    */

    std::unordered_map<std::string, int> O_hedge_map; // obstacle dcel hedges key, index
    std::vector<HEdge*> O_hedge_vec;
    std::vector<Face*> O_Faces = D_obs->getFaces();

    for (size_t i = 0; i < O_Faces.size(); i++) {
        std::vector<HEdge*> O_Face_Inner_Hedge_Vec = O_Faces[i]->getInnerHEdges();
        for (size_t j = 0; j < O_Face_Inner_Hedge_Vec.size(); j++) {
            O_hedge_map[O_Face_Inner_Hedge_Vec[j]->getKey()] = O_hedge_vec.size();
            O_hedge_map[O_Face_Inner_Hedge_Vec[j]->getTwin()->getKey()] = O_hedge_vec.size();
            O_hedge_vec.push_back(O_Face_Inner_Hedge_Vec[j]);
        }
    }

    std::vector<std::vector<std::pair<Point, int>>> new_obs_point_list(O_hedge_vec.size(),std::vector<std::pair<Point, int>>());
    std::unordered_map<int, int> new_TRP_vertices_map;
    std::unordered_map<int, int> exception_TRP_vertices_map;
    std::vector<Vertex*> new_TRP_vertices;
    int errcntRay[4] = { 0,0,0,0 };
    // For each vertex of S, reflect point location query result
    for (size_t i = 0; i < point_location_query.size(); i++) {
        for (int j = 0; j < 2; j++) {
            if (V_S_Data[i].isNull[j]) continue;
            if (point_location_query[i].answer[j] == NULL) continue;
            Point intersection = verticalRayIntersection(s_vertices[i]->getPoint(), point_location_query[i].answer[j]->getEdge());
            if (j == 0) {
                //debug
                //if (std::abs(s_vertices[i]->getPoint().getx() - intersection.getx()) > TOLERANCE_CS_FREE) errcntRay[0]++;
                //if (s_vertices[i]->getPoint().gety() > intersection.gety()) errcnt++;
                if (intersection.gety() < V_S_Data[i].neighbor[j].gety()) {                    
                    V_S_Data[i].isChanged[j] = true;
                    Vertex* new_v = new Vertex(intersection);
                    new_v->setKey("A_"+std::to_string(new_TRP_vertex_index));
                    new_TRP_vertices.push_back(new_v);
                    new_TRP_vertices_map[new_TRP_vertex_index] = new_TRP_vertex_index;
                    //exeception handling start
                    if (std::abs(intersection.gety() - V_S_Data[i].neighbor[j].gety()) < TOLERANCE_CS_FREE) {
                        exception_TRP_vertices_map[new_TRP_vertex_index] = V_S_Data[i].v_idx[j];
                    }
                    V_S_Data[i].neighbor[j] = intersection;
                    V_S_Data[i].new_v_idx[j] = new_TRP_vertex_index;
                    //exeception handling end 
                    new_obs_point_list[O_hedge_map[point_location_query[i].answer[j]->getKey()]].push_back(std::make_pair(intersection, new_TRP_vertex_index));
                    new_TRP_vertex_index++;
                }
            }
            else {
                //debug
                //if (std::abs(s_vertices[i]->getPoint().getx() - intersection.getx()) > TOLERANCE_CS_FREE) errcntRay[1]++;
                //if (s_vertices[i]->getPoint().gety() < intersection.gety()) errcnt++;
                if (intersection.gety() > V_S_Data[i].neighbor[j].gety()) {                    
                    V_S_Data[i].isChanged[j] = true;
                    Vertex* new_v = new Vertex(intersection);
                    new_v->setKey("A_" + std::to_string(new_TRP_vertex_index));
                    new_TRP_vertices.push_back(new_v);
                    new_TRP_vertices_map[new_TRP_vertex_index] = new_TRP_vertex_index;
                    //exeception handling start
                    if (std::abs(intersection.gety() - V_S_Data[i].neighbor[j].gety()) < TOLERANCE_CS_FREE) {
                        exception_TRP_vertices_map[new_TRP_vertex_index] = V_S_Data[i].v_idx[j];
                    }
                    V_S_Data[i].neighbor[j] = intersection;
                    V_S_Data[i].new_v_idx[j] = new_TRP_vertex_index;
                    //exeception handling end                   
                    new_obs_point_list[O_hedge_map[point_location_query[i].answer[j]->getKey()]].push_back(std::make_pair(intersection, new_TRP_vertex_index));
                    new_TRP_vertex_index++;
                }
            }
        }
        for (int j = 2; j < 4; j++) {
            if (V_S_Data[i].isNull[j]) continue;
            if (point_location_query[i].answer[j] == NULL) continue;
            Point intersection = horizontalRayIntersection(s_vertices[i]->getPoint(), point_location_query[i].answer[j]->getEdge());
            /*
            if (i == 2330 || i == 3303) {
                std::cout << i << ' ' << j << ' ' << intersection.getx() << ' ' << intersection.gety() << ' ' << point_location_query[i].answer[j]->getKey() << ' ' << point_location_query[i].answer[j]->getTwin()->getKey() << std::endl;
                std::cout << i << ' ' << j << ' ' << s_vertices[i]->getx() << ' ' << s_vertices[i]->gety() << std::endl;
            }
            */
            if (j == 2) {
                //debug
                //if (std::abs(s_vertices[i]->getPoint().gety() - intersection.gety()) > TOLERANCE_CS_FREE) errcntRay[2]++;
                //if (s_vertices[i]->getPoint().getx() > intersection.getx()) errcnt++;
                /*
                if (i == 1629) {
                    std::cout << intersection.getx() << ' ' << V_S_Data[i].neighbor[j].getx() << std::endl;
                }
                */
                if (intersection.getx() < V_S_Data[i].neighbor[j].getx()) {                    
                    V_S_Data[i].isChanged[j] = true;
                    Vertex* new_v = new Vertex(intersection);
                    new_v->setKey("A_" + std::to_string(new_TRP_vertex_index));
                    new_TRP_vertices.push_back(new_v);
                    new_TRP_vertices_map[new_TRP_vertex_index] = new_TRP_vertex_index;
                    //exeception handling start
                    if (std::abs(intersection.getx() - V_S_Data[i].neighbor[j].getx()) < TOLERANCE_CS_FREE) {
                        exception_TRP_vertices_map[new_TRP_vertex_index] = V_S_Data[i].v_idx[j];
                    }
                    V_S_Data[i].neighbor[j] = intersection;
                    V_S_Data[i].new_v_idx[j] = new_TRP_vertex_index;
                    //exeception handling end                    
                    new_obs_point_list[O_hedge_map[point_location_query[i].answer[j]->getKey()]].push_back(std::make_pair(intersection, new_TRP_vertex_index));
                    new_TRP_vertex_index++;
                }
            }
            else {
                //debug
                //if (std::abs(s_vertices[i]->getPoint().gety() - intersection.gety()) > TOLERANCE_CS_FREE) errcntRay[3]++;
                //if (s_vertices[i]->getPoint().getx() < intersection.getx()) errcnt++;
                if (intersection.getx() > V_S_Data[i].neighbor[j].getx()) {                    
                    V_S_Data[i].isChanged[j] = true;
                    Vertex* new_v = new Vertex(intersection);
                    new_v->setKey("A_" + std::to_string(new_TRP_vertex_index));
                    new_TRP_vertices.push_back(new_v);
                    new_TRP_vertices_map[new_TRP_vertex_index] = new_TRP_vertex_index;
                    //exeception handling start
                    if (std::abs(intersection.getx() - V_S_Data[i].neighbor[j].getx()) < TOLERANCE_CS_FREE) {
                        exception_TRP_vertices_map[new_TRP_vertex_index] = V_S_Data[i].v_idx[j];
                    }
                    V_S_Data[i].neighbor[j] = intersection;
                    V_S_Data[i].new_v_idx[j] = new_TRP_vertex_index;
                    //exeception handling end                    
                    new_obs_point_list[O_hedge_map[point_location_query[i].answer[j]->getKey()]].push_back(std::make_pair(intersection, new_TRP_vertex_index));
                    new_TRP_vertex_index++;
                }
            }
        }
    }   
    
    for (size_t i = 0; i < O_hedge_vec.size(); i++) {
        Point origin = O_hedge_vec[i]->getOrigin()->getPoint();

        if (!new_obs_point_list[i].empty()) {
            if (std::abs(origin.getx() - new_obs_point_list[i].front().first.getx()) < TOLERANCE_CS_FREE) {
                if (origin.gety() < new_obs_point_list[i].front().first.gety()) {
                    std::sort(new_obs_point_list[i].begin(), new_obs_point_list[i].end(), comparePoint1);
                }
                else {
                    std::sort(new_obs_point_list[i].begin(), new_obs_point_list[i].end(), comparePoint2);
                }
                // 중복 삭제
                for (size_t j = 0; j < new_obs_point_list[i].size() - 1; j++) {
                    if (std::abs(new_obs_point_list[i][j].first.gety() - new_obs_point_list[i][j + 1].first.gety()) < TOLERANCE_CS_FREE) {
                        new_TRP_vertices_map[new_obs_point_list[i][j + 1].second] = new_TRP_vertices_map[new_obs_point_list[i][j].second];
                    }
                }
            }
            else {
                if (origin.getx() < new_obs_point_list[i].front().first.getx()) {
                    std::sort(new_obs_point_list[i].begin(), new_obs_point_list[i].end(), comparePoint3);
                }
                else {
                    std::sort(new_obs_point_list[i].begin(), new_obs_point_list[i].end(), comparePoint4);
                }
                // 중복 삭제
                for (size_t j = 0; j < new_obs_point_list[i].size() - 1; j++) {
                    if (std::abs(new_obs_point_list[i][j].first.getx() - new_obs_point_list[i][j + 1].first.getx()) < TOLERANCE_CS_FREE) {
                        new_TRP_vertices_map[new_obs_point_list[i][j + 1].second] = new_TRP_vertices_map[new_obs_point_list[i][j].second];
                    }
                }                 
            }
        }
    }
    
    int G_V_idx = G_V.size();
    std::unordered_map<std::string, int> O_V_map;
    std::unordered_map<std::string, bool> O_V_duplication_check;
    // Point(Vertex) List update 1
    //std::cout << O_hedge_vec.size() << std::endl;
    for (size_t i = 0; i < O_hedge_vec.size(); i++) {
        Vertex* obsV = O_hedge_vec[i]->getOrigin();
        if (O_V_duplication_check.find(obsV->getKey()) == O_V_duplication_check.end()) {
            O_V_duplication_check[obsV->getKey()] = true;
            Point origin = O_hedge_vec[i]->getOrigin()->getPoint();
            G_V.push_back(origin);
            Vertex* v = new Vertex(origin);
            std::string v_key = "O_" + std::to_string(G_V_idx);
            //this->vertices_types[v_key] = V_OBS;
            G_V_map[v_key] = G_V_idx;
            O_V_map[O_hedge_vec[i]->getOrigin()->getKey()] = G_V_idx;
            G_V_idx++;
        }
    }

    // Point(Vertex) List update 2
    for (auto it = new_TRP_vertices_map.begin(); it != new_TRP_vertices_map.end(); it++) {
        if (it->first == it->second) {
            if (exception_TRP_vertices_map.find(it->second) == exception_TRP_vertices_map.end()) {
                G_V.push_back(new_TRP_vertices[it->second]->getPoint());
                //this->vertices_types[new_TRP_vertices[it->second]->getKey()] = V_TRP;
                G_V_map[new_TRP_vertices[it->second]->getKey()] = G_V_idx;
                G_V_idx++;
            }          
        }
    }
    
    //debug
    /*
    for (int i = 0; i < G_V.size() - 1; i++) {
        for (int j = i + 1; j < G_V.size(); j++) {
            if (std::abs(G_V[i].getx() - G_V[j].getx()) < 1e-12 &&
                std::abs(G_V[i].gety() - G_V[j].gety()) < 1e-12) {
                std::cout << "Degenerate case!\n";
            }
        }
    }
    */

    // Edge List update 1
    std::vector<int> testest(G_V_map.size(), 0);
    std::vector<std::vector<int>> G_E(G_V.size(), std::vector<int>());
    for (size_t i = 0; i < V_S_Data.size(); i++) {
        for (int j = 0; j < 4; j++) {
            if (V_S_Data[i].isNull[j]) continue;
            if (V_S_Data[i].isChanged[j]) {
                int nextidx;
                if (exception_TRP_vertices_map.find(new_TRP_vertices_map[V_S_Data[i].new_v_idx[j]]) == exception_TRP_vertices_map.end()) {                    
                    nextidx = G_V_map[new_TRP_vertices[new_TRP_vertices_map[V_S_Data[i].new_v_idx[j]]]->getKey()];
                    G_E[nextidx].push_back(i);
                    testest[nextidx]++;
                    //std::cout << i << ' ' << nextidx << std::endl;
                }
                else {
                    //std::cout << "Exception handling: " << i << ' ' << new_TRP_vertices_map[V_S_Data[i].new_v_idx[j]] << std::endl;
                    nextidx = V_S_Data[i].v_idx[j];
                }                
                G_E[i].push_back(nextidx);
                /*debug
                if (std::abs(G_V[i].getx() - G_V[nextidx].getx()) < TOLERANCE_CS_FREE && std::abs(G_V[i].gety() - G_V[nextidx].gety()) < TOLERANCE_CS_FREE)
                    std::cout << i << ' ' << nextidx << std::endl;
                */
            }
            else {
                G_E[i].push_back(V_S_Data[i].v_idx[j]);
                //G_E[V_S_Data[i].v_idx[j]].push_back(i);
            }
        }
    }
    
    // debug
    /*
    std::cout << G_V[1629].getx() << ' ' << G_V[1629].gety() << std::endl;
    for (int i = 0; i < 4; i++) {
        //std::cout << V_S_Data[1629][i] << std::endl;
        std::cout << point_location_query[1629].answer[i] << std::endl;
    }
    */

    //debug
    /*
    std::cout << G_V_map[new_TRP_vertices[new_TRP_vertices_map[V_S_Data[3303].new_v_idx[3]]]->getKey()] << std::endl;
    std::cout << V_S_Data[3303].v_idx[3] << std::endl;
    std::cout << V_S_Data[V_S_Data[3303].v_idx[3]].isChanged[2] << std::endl;
    std::cout << V_S_Data[V_S_Data[3303].v_idx[3]].isNull[2] << std::endl;
    std::cout << V_S_Data[V_S_Data[3303].v_idx[3]].v_idx[2] << std::endl;
    std::cout << V_S_Data[V_S_Data[3303].v_idx[3]].new_v_idx[2] << std::endl;
    for (int i = 0; i < testest.size(); i++) {
        if (testest[i] == 1) std::cout << i << std::endl;
    }
    */

    // Edge List update 2
    for (size_t i = 0; i < O_hedge_vec.size(); i++) {
        // First
        int prevIdx = O_V_map[O_hedge_vec[i]->getOrigin()->getKey()];
        int nowIdx;
        if (!new_obs_point_list[i].empty()) {
            if (exception_TRP_vertices_map.find(new_TRP_vertices_map[new_obs_point_list[i].front().second]) == exception_TRP_vertices_map.end()) {
                nowIdx = G_V_map[new_TRP_vertices[new_TRP_vertices_map[new_obs_point_list[i].front().second]]->getKey()];
            }
            else {
                nowIdx = exception_TRP_vertices_map[new_obs_point_list[i].front().second];
            }                       
            G_E[prevIdx].push_back(nowIdx);
            G_E[nowIdx].push_back(prevIdx);
            prevIdx = nowIdx;           
            for (size_t j = 1; j < new_obs_point_list[i].size(); j++) {
                if (new_TRP_vertices_map[new_obs_point_list[i][j - 1].second] != new_TRP_vertices_map[new_obs_point_list[i][j].second]) {
                    if (exception_TRP_vertices_map.find(new_TRP_vertices_map[new_obs_point_list[i][j].second]) == exception_TRP_vertices_map.end()) {
                        nowIdx = G_V_map[new_TRP_vertices[new_TRP_vertices_map[new_obs_point_list[i][j].second]]->getKey()];
                    }
                    else {
                        nowIdx = exception_TRP_vertices_map[new_TRP_vertices_map[new_obs_point_list[i][j].second]];
                    }                    
                    G_E[prevIdx].push_back(nowIdx);
                    G_E[nowIdx].push_back(prevIdx);
                    prevIdx = nowIdx;
                }
            }          
        }
        // Last
        nowIdx = O_V_map[O_hedge_vec[i]->getNext()->getOrigin()->getKey()];
        G_E[prevIdx].push_back(nowIdx);
        G_E[nowIdx].push_back(prevIdx);
    }
    

    // Construct graph for result DCEL
    std::vector<std::pair<std::string, Point>> G_V_key;
    std::vector<std::pair<int, std::string>> G_V_key_idx;
    for (auto it = G_V_map.begin(); it != G_V_map.end(); it++) {
        G_V_key_idx.push_back(std::make_pair(it->second, it->first));
    }
    std::sort(G_V_key_idx.begin(), G_V_key_idx.end());
    for (size_t i = 0; i < G_V_key_idx.size(); i++) {
        G_V_key.push_back(std::make_pair(G_V_key_idx[i].second, G_V[i]));
    }


    // debug
    /*
    for (auto it = G_V_map.begin(); it != G_V_map.end(); it++) {
        //5.01562
        if (std::abs(5.0 - G_V[it->second].getx()) < TOLERANCE_CS_FREE_TEMP &&
            std::abs(1.125 - G_V[it->second].gety()) < TOLERANCE_CS_FREE_TEMP) {
            std::cout << "debug" << ' ' << it->second << std::endl;
            for (int i = 0; i < G_E[it->second].size(); i++) {
                std::cout << G_V[G_E[it->second][i]].getx() << ' ' << G_V[G_E[it->second][i]].gety() << std::endl;
            }          
            //std::cout << V_S_Data[it->second].neighbor[0].getx() << ' ' << V_S_Data[it->second].neighbor[0].gety() << std::endl;
            //std::cout << V_S_Data[it->second].neighbor[1].getx() << ' ' << V_S_Data[it->second].neighbor[1].gety() << std::endl;
            //std::cout << V_S_Data[it->second].neighbor[2].getx() << ' ' << V_S_Data[it->second].neighbor[2].gety() << std::endl;
            //std::cout << V_S_Data[it->second].neighbor[3].getx() << ' ' << V_S_Data[it->second].neighbor[3].gety() << std::endl;            
        }
    }
    */

    DCEL S_overlay(G_V_key, G_E);
    
    // debugging
    /*
    std::vector<Point> G_V_Temp; // debug
    std::vector<std::pair<Point, int>> G_V_Temp_ori; // debug
    for (auto it = G_V_map.begin(); it != G_V_map.end(); it++) {
        G_V_Temp_ori.push_back(std::make_pair(G_V[it->second], it->second));
    }
    std::sort(G_V_Temp_ori.begin(), G_V_Temp_ori.end(), sort_by_idx);
    for (auto gv : G_V_Temp_ori) {
        G_V_Temp.push_back(gv.first);
    }
    print_result(&G_V_Temp, &G_V_map, &G_E, "CS_Free.txt"); // debug
    */

    

    /* debugging
    for (auto a : G_V_key) {
        std::string key = a.first;
        std::cout << key << std::endl;
    }
    
    std::vector<HEdge*> edges = S_overlay.getHedges();
    std::vector<Vertex*> vertices = S_overlay.getVertices();
    std::cout << edges.size() << std::endl;
    std::cout << vertices.size() << std::endl;
    */
    std::cout << "checkpoint5\n";

    // 6. Store vertices type and half edge types by using labels of S_overlay.
    {
        std::vector<HEdge*> edges = S_overlay.getHedges();
        std::vector<Vertex*> vertices = S_overlay.getVertices();

        /* debugging
        std::cout << edges.size() << std::endl;
        std::cout << vertices.size() << std::endl;      
        for (size_t i = 0; i < vertices.size(); i++) {
            std::cout << i << std::endl;
            std::string key = vertices[i]->getKey();
            std::cout << key << std::endl;
        }
        for (size_t i = 0; i < edges.size(); i++) {
            std::cout << i << std::endl;
            std::string key = edges[i]->getKey();
            std::cout << key << std::endl;
        }
        */

        for (auto e : edges) {
            std::string key = e->getKey(); // Check this code
            //std::cout << key << std::endl;
            std::string u = e->getOrigin()->getKey();
            std::string v = e->getNext()->getOrigin()->getKey();
            if (v.front() == 'v' || u.front() == 'v') {
                this->edge_types[key] = HE_TRP;
            }
            else {
                this->edge_types[key] = HE_OPQ;
            }
        }
        std::cout << "checkpoint6-1\n";
        for (auto v : vertices) {
            std::string key = v->getKey();
            Point pos = v->getPoint();
            //src
            if (std::abs(src.getx() - pos.getx()) < TOLERANCE_CS_FREE && std::abs(src.gety() - pos.gety()) < TOLERANCE_CS_FREE) {
                this->vertices_types[key] = V_SRC;
            }
            else if (key.front() == 'O') {              
                /*
                if (this->vertices_types.find(key) == this->vertices_types.end()) { 
                    std::cout << key << ' ' << v->getx() << ' ' << v->gety() << std::endl;
                }
                */
                this->vertices_types[key] = V_OBS;                
            }
            else {
                this->vertices_types[key] = V_TRP;
            }
        }
    }
    std::cout << "checkpoint6-2\n";

    //debug
    /*
    int vobscnt = 0;
    for (auto v : this->vertices_types) {
        if (v.second == V_OBS) vobscnt++;
    }
    std::cout << this->vertices_types.size() << std::endl;
    std::cout << vobscnt << std::endl;
    */

    //7. Partition each cell containing an obstacle vertex v by extending edges vertically up and down from v.
    // HOW: interesting cell일 것이고, 새로운 vertex, half edge 추가, graph로 만들어서 이를 다시 DCEL로.
    // DCEL: inner CW / outer CCW
    {
        std::vector<Face*> added_faces;
        std::vector<Face*> faces = S_overlay.getFaces();
        std::vector<double> deltas;
        // Compute delta
        for (auto f : faces) {
            std::vector<HEdge*> edges = f->getOutHEdges();
            double delta = DBL_MAX;
            for (auto he : edges) {
                if (this->vertices_types[he->getOrigin()->getKey()] == V_TRP &&
                    this->vertices_types[he->getNext()->getOrigin()->getKey()] == V_TRP) {
                    double temp = he->getOrigin()->getPoint().distance(he->getNext()->getOrigin()->getPoint());
                    if (delta > temp) delta = temp;
                }
            }
            deltas.push_back(delta);
        }
        std::cout << "checkpoint7-1\n";
        int new_vertex_idx = 0; // V_
        //std::cout << faces.size() << std::endl;
        
        // debug
        /*
        int debugObs = 0;
        int debugInner = 0;
        std::cout << faces.size() << std::endl;
        for (size_t i = 0; i < faces.size(); i++) {
            auto f = faces[i];
            std::cout << i << ' ' << f->getInnerHEdges().size() << ' ' << f->getOutVertices().size() << ' ' << std::endl;
            if (f->getInnerHEdges().size() > 0) debugInner++;
            //for (size_t j = 0; j < f.)
        }
        std::cout << "Inner face: " << debugInner << std::endl;
        */

        //debug
        /*
        std::unordered_map<string, int> debug_print_G_V_MAP;
        std::vector<Point> debug_print_G_V;
        std::vector<Vertex*> debug_print_V = S_overlay.getVertices();
        int debug_print_v_idx = 0;
        for (size_t i = 0; i < debug_print_V.size(); i++) {
            if (debug_print_G_V_MAP.find(debug_print_V[i]->getKey()) == debug_print_G_V_MAP.end()) {
                debug_print_G_V_MAP[debug_print_V[i]->getKey()] = debug_print_v_idx++;
                debug_print_G_V.push_back(debug_print_V[i]->getPoint());
                //std::cout << debug_print_V[i]->getKey() << std::endl;
            }
        }
     
        std::vector<std::vector<int>> debug_print_G_E(debug_print_G_V.size(), std::vector<int>());
        //for (size_t i = 0; i < faces.size(); i++) {
            auto f = faces[350];
            std::vector<HEdge*> hedges = f->getInnerHEdges();
            //std::cout << "check\n";
            
            for (auto he : hedges) {
                int u = debug_print_G_V_MAP[he->getOrigin()->getKey()];                
                int v = debug_print_G_V_MAP[he->getNext()->getOrigin()->getKey()];
                debug_print_G_E[u].push_back(v);
                debug_print_G_E[v].push_back(u);
                std::cout << he->getOrigin()->getx() << ' ' << he->getOrigin()->gety() << std::endl;
            }
            
            std::vector<HEdge*> hedges2 = f->getOutHEdges();
            for (auto he : hedges2) {
                int u = debug_print_G_V_MAP[he->getOrigin()->getKey()];
                int v = debug_print_G_V_MAP[he->getNext()->getOrigin()->getKey()];
                debug_print_G_E[u].push_back(v);
                debug_print_G_E[v].push_back(u);
            } 
            
        //}

        print_result(&debug_print_G_V, &debug_print_G_E,"CS_Free.txt");
        */

        //debug
        /*
        for (size_t i = 0; i < faces.size(); i++) {
            auto f = faces[i];
            std::vector<Vertex*> vertices = f->getOutVertices();
            for (auto v : vertices) {
                if (this->vertices_types[v->getKey()] == V_OBS) {
                    std::cout << v->getKey() << std::endl;
                }
            }
        }
        */

        //debug
        /*
        auto f = faces[1];
        std::vector<HEdge*> debugedges = f->getOutHEdges();
        HEdge* debugHedge = debugedges[0];
        for (int i = 0; i < debugedges.size(); i++) {
            std::cout << debugHedge->getOrigin()->getx() << ' ' << debugHedge->getOrigin()->gety() << std::endl;
            debugHedge = debugHedge->getNext();
        }
        */

        for (size_t i = 0; i < faces.size(); i++) {
            //std::cout << "checkpoint7-2-" << i << "\n";
            auto f = faces[i];
            //std::vector<Vertex*> vertices = f->getInnerVertices(0);
            std::vector<Vertex*> vertices = f->getOutVertices();
            //std::cout << "check\n";
            for (auto v : vertices) {
                if (this->vertices_types[v->getKey()] == V_OBS) {
                    //debugObs++;
                    // Exception handling variables
                    bool isUpperRayIntersect = false; // If false, upper ray does not partition interesting cell 
                    bool isLowerRayIntersect = false; // If false, lower ray does not partition interesting cell 
                    bool isUpperIntersectVertex = true;
                    bool isLowerIntersectVertex = true;

                    //std::vector<HEdge*> edges = f->getInnerHEdges();
                    std::vector<HEdge*> edges = f->getOutHEdges();
                    HEdge* het;
                    HEdge* heb;
                    if (!edges.empty()) {
                        HEdge* startHedge = edges[0];
                        HEdge* he = startHedge;                       
                        // obstacle vertex 기준 잘라야할 edge 찾기 (위,아래)
                        do {
                            //Edge e = he->getEdge();

                            //Point es = e.gets();
                            Point es = he->getOrigin()->getPoint();
                            //Point et = e.gett();
                            Point et = he->getNext()->getOrigin()->getPoint();
                            // 예외처리 (v 포함하는 edge일 경우)
                            if (std::abs(es.getx() - v->getx()) < TOLERANCE_CS_FREE &&
                                std::abs(es.gety() - v->gety()) < TOLERANCE_CS_FREE ) {
                                he = he->getNext();
                                continue;
                            }
                            if (std::abs(et.getx() - v->getx()) < TOLERANCE_CS_FREE &&
                                std::abs(et.gety() - v->gety()) < TOLERANCE_CS_FREE) {
                                he = he->getNext();
                                continue;
                            }

                            // v 위에 있는 edge 찾기 (부등호 예외 처리 확인)
                            if (es.getx() > et.getx()) {
                                if (std::abs(es.getx() - v->getx()) < TOLERANCE_CS_FREE && v->getx() > et.getx()) {
                                    het = he;
                                    if (std::abs(es.getx() - v->getx()) > TOLERANCE_CS_FREE) {
                                        isUpperIntersectVertex = false;
                                    }
                                    isUpperRayIntersect = true;
                                }
                            }
                            // v 아래 있는 edge 찾기
                            else {
                                if (std::abs(es.getx() - v->getx()) < TOLERANCE_CS_FREE && v->getx() < et.getx()) {
                                    heb = he;
                                    if (std::abs(es.getx() - v->getx()) > TOLERANCE_CS_FREE) {
                                        isLowerIntersectVertex = false;
                                    }
                                    isLowerRayIntersect = true;
                                }
                            }
                            he = he->getNext();
                        } while (std::abs(he->getOrigin()->getx() - startHedge->getOrigin()->getx()) > TOLERANCE_CS_FREE || 
                            std::abs(he->getOrigin()->gety() - startHedge->getOrigin()->gety()) > TOLERANCE_CS_FREE);
                    }
                    std::cout << isUpperRayIntersect << ' ' << isUpperIntersectVertex << ' ' << isLowerRayIntersect << ' ' << isLowerIntersectVertex << std::endl;
                    std::cout << "checkpoint7-2-*-1\n";

                    // Make new edges
                    std::vector<HEdge*> upperLeft, upperRight, lowerLeft, lowerRight; // index가 작을수록 obstacle vertex와 가까움
                    Point topIntersectionPoint, bottomIntersectionPoint;
                    int upperEdgeNumber, lowerEdgeNumber;
                    double upperEdgeLength, lowerEdgeLength;
                    std::vector<Vertex*> upperVertices, lowerVertices;
                    if (isUpperRayIntersect) {
                        topIntersectionPoint = verticalRayIntersection(v->getPoint(), het->getEdge());
                        std::cout << v->getPoint().getx() << ' ' << v->getPoint().gety() << std::endl;
                        std::cout << het->getEdge().gets().getx() << ' ' << het->getEdge().gets().gety() <<  ' ' << het->getEdge().gett().getx() << ' ' << het->getEdge().gett().gety() << std::endl;
                        std::cout << topIntersectionPoint.getx() << ' ' << topIntersectionPoint.gety() << ' ' << deltas[i] << std::endl;
                        upperEdgeNumber = std::floor((topIntersectionPoint.gety() - std::abs(v->getPoint().gety())) / deltas[i]);
                        upperEdgeLength = std::abs(v->getPoint().gety() - topIntersectionPoint.gety()) / double(upperEdgeNumber);
                        upperVertices.push_back(v);
                    }
                    if (isLowerRayIntersect) {
                        bottomIntersectionPoint = verticalRayIntersection(v->getPoint(), heb->getEdge());
                        lowerEdgeNumber = std::floor((std::abs(v->getPoint().gety() - bottomIntersectionPoint.gety())) / deltas[i]);
                        lowerEdgeLength = std::abs(v->getPoint().gety() - bottomIntersectionPoint.gety()) / double(lowerEdgeNumber);
                        lowerVertices.push_back(v);
                    }                                        
                    std::cout << "checkpoint7-2-*-2\n";

                    // Make upper hedges and Set Twin
                    if (isUpperRayIntersect) {
                        std::cout << "upperEdgeNumber:" << upperEdgeNumber << std::endl;
                        for (int i = 0; i < upperEdgeNumber; i++) {
                            Vertex* newV = new Vertex(v->getx(), v->gety() + upperEdgeLength * (i+1));
                            newV->setKey("NV_" + std::to_string(new_vertex_idx++));
                            this->vertices_types[newV->getKey()] = V_TRP;
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
                            upperLeft[i]->setNext(upperLeft[i + 1]);
                            upperLeft[i + 1]->setPrev(upperLeft[i]);
                            upperRight[i + 1]->setNext(upperRight[i]);
                            upperRight[i]->setPrev(upperRight[i + 1]);
                        }
                    }                                       
                    std::cout << "checkpoint7-2-*-4\n";

                    // Make lower hedges and Set twin
                    if (isLowerRayIntersect) {
                        for (int i = 0; i < lowerEdgeNumber; i++) {
                            Vertex* newV = new Vertex(v->getx(), v->gety() - lowerEdgeLength * (i + 1));
                            newV->setKey("NV_" + std::to_string(new_vertex_idx++));
                            this->vertices_types[newV->getKey()] = V_TRP;
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
                    }                    
                    std::cout << "checkpoint7-2-*-6\n";

                    //std::vector<HEdge*> edges = f->getInnerHEdges();
                    HEdge* he = edges[0];
                    HEdge* firstEdge; // whose origin is obstacle vertex
                    while (true) {
                        Vertex nowV = *he->getOrigin();
                        if (std::abs(v->getx() - nowV.getx()) < TOLERANCE_CS_FREE && std::abs(v->gety() - nowV.gety()) < TOLERANCE_CS_FREE) {
                            firstEdge = he;
                            break;
                        }
                        he = he->getNext();
                    }
                    std::cout << "checkpoint7-2-*-7\n";

                    bool isUpper = false; // If Hedge whose origin is obstacle vertex that is included in new upper cell then true
                    HEdge* upFaceFirst, *loFaceFirst, *upFaceLast, *loFaceLast;
                    HEdge* originFaceFirst, *originFaceLast;
                    HEdge* nowHEdge = firstEdge;
                    /*
                    //HEdge* nextEdge = firstEdge->getNext();
                    Edge e_het, e_bet;
                    if (isUpperRayIntersect) {
                        e_het = het->getEdge();
                        //e_het.sets(het->getEdge().gets());
                        //e_het.sett(het->getEdge().gett());
                    }
                    if (isLowerRayIntersect) {
                        e_bet = heb->getEdge();
                        //e_bet.sets(heb->getEdge().gets());
                        //e_bet.sett(heb->getEdge().gett());
                    }*/
                    
                    if (isUpperRayIntersect && isLowerRayIntersect) {
                        while (true) {
                            nowHEdge = nowHEdge->getNext();
                            //nextEdge = nowEdge->getNext();
                            Edge nowEdge = nowHEdge->getEdge();
                            /*
                            if (std::abs(nowEdge.gets().getx() - e_het.gets().getx()) < TOLERANCE_CS_FREE &&
                                std::abs(nowEdge.gets().gety() - e_het.gets().gety()) < TOLERANCE_CS_FREE &&
                                std::abs(nowEdge.gett().getx() - e_het.gett().getx()) < TOLERANCE_CS_FREE &&
                                std::abs(nowEdge.gett().gety() - e_het.gett().gety()) < TOLERANCE_CS_FREE) */
                            if (std::abs(nowHEdge->getOrigin()->getx() - het->getOrigin()->getx()) < TOLERANCE_CS_FREE &&
                                std::abs(nowHEdge->getOrigin()->gety() - het->getOrigin()->getx()) < TOLERANCE_CS_FREE ) {
                                isUpper = true;
                                std::cout << "checkpoint7-2-*-upper\n";

                                if (isUpperIntersectVertex) {
                                    std::cout << "checkpoint7-2-*-upper-up-vertex\n";
                                    upFaceFirst = firstEdge;
                                    upFaceLast = nowHEdge->getPrev();
                                    originFaceFirst = nowHEdge;
                                }

                                else {
                                    std::cout << "checkpoint7-2-*-upper-up-edge\n";
                                    // Split original edges                           
                                    upFaceFirst = firstEdge;
                                    upFaceLast = nowHEdge;
                                    originFaceFirst = new HEdge();
                                    originFaceFirst->setOrigin(upperVertices.back());
                                    originFaceFirst->setNext(nowHEdge->getNext());
                                    originFaceFirst->getNext()->setPrev(originFaceFirst);

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
                                }

                                if (isLowerIntersectVertex) {
                                    std::cout << "checkpoint7-2-*-upper-lo-vertex\n";
                                    loFaceFirst = heb;
                                    loFaceLast = firstEdge->getPrev();
                                    originFaceLast = heb->getPrev();
                                }

                                else {
                                    std::cout << "checkpoint7-2-*-upper-lo-edge\n";
                                    loFaceLast = firstEdge->getPrev();
                                    // Split original edges                                    
                                    loFaceFirst = new HEdge();
                                    originFaceLast = heb;
                                    loFaceFirst->setOrigin(lowerVertices.back());
                                    loFaceFirst->setNext(originFaceLast->getNext());
                                    loFaceFirst->getNext()->setPrev(loFaceFirst);

                                    // Update DCEL of neighbor cell
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
                                }
                                break;
                            }
                            /*
                            else if (std::abs(nowEdge.gets().getx() - e_bet.gets().getx()) < TOLERANCE_CS_FREE &&
                                std::abs(nowEdge.gets().gety() - e_bet.gets().gety()) < TOLERANCE_CS_FREE &&
                                std::abs(nowEdge.gett().getx() - e_bet.gett().getx()) < TOLERANCE_CS_FREE &&
                                std::abs(nowEdge.gett().gety() - e_bet.gett().gety()) < TOLERANCE_CS_FREE)*/
                            else if (std::abs(nowHEdge->getOrigin()->getx() - heb->getOrigin()->getx()) < TOLERANCE_CS_FREE &&
                                std::abs(nowHEdge->getOrigin()->gety() - heb->getOrigin()->gety()) < TOLERANCE_CS_FREE) {
                                std::cout << "checkpoint7-2-*-lower\n";

                                //std::cout << nowHEdge->getOrigin()->getx() << ' ' << nowHEdge->getOrigin()->gety() << std::endl;
                                //std::cout << nowHEdge->getNext()->getOrigin()->getx() << ' ' << nowHEdge->getNext()->getOrigin()->gety() << std::endl;
                                if (isLowerIntersectVertex) {
                                    std::cout << "checkpoint7-2-*-lower-lo-vertex\n";
                                    loFaceFirst = firstEdge;
                                    loFaceLast = nowHEdge->getPrev();
                                    originFaceFirst = nowHEdge;
                                }

                                else {
                                    std::cout << "checkpoint7-2-*-lower-lo-edge\n";
                                    // Split original edges
                                    loFaceFirst = firstEdge;
                                    loFaceLast = nowHEdge;
                                    originFaceFirst = new HEdge();
                                    originFaceFirst->setOrigin(lowerVertices.back());
                                    originFaceFirst->setNext(nowHEdge->getNext());
                                    originFaceFirst->getNext()->setPrev(originFaceFirst);

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
                                }     

                                if (isUpperIntersectVertex) {
                                    upFaceFirst = het;
                                    upFaceLast = firstEdge->getPrev();
                                    originFaceLast = het->getPrev();
                                }

                                else {
                                    upFaceLast = firstEdge->getPrev();
                                    // Split original edges
                                    upFaceFirst = new HEdge();
                                    originFaceLast = het;
                                    upFaceFirst->setOrigin(upperVertices.back());
                                    upFaceFirst->setNext(originFaceLast->getNext());
                                    upFaceFirst->getNext()->setPrev(upFaceFirst);

                                    // Update DCEL of neighbor cell
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
                                }

                                break;
                            }
                        }
                    
                        std::cout << upperLeft.size() << std::endl;
                        if (isUpper) {
                            originFaceFirst->setPrev(upperLeft.back());
                            upperLeft.back()->setNext(originFaceFirst);
                            lowerLeft.front()->setNext(upperLeft.front());
                            upperLeft.front()->setPrev(lowerLeft.front());
                            originFaceLast->setNext(lowerLeft.back());
                            lowerLeft.back()->setPrev(originFaceLast);
                        }
                        else {
                            originFaceFirst->setPrev(lowerRight.back());
                            lowerRight.back()->setNext(originFaceFirst);
                            upperRight.front()->setNext(lowerRight.front());
                            lowerRight.front()->setPrev(upperRight.front());
                            originFaceLast->setNext(upperRight.back());
                            upperRight.back()->setPrev(originFaceLast);
                        }
                        //std::vector<HEdge*> fHedgeList;
                        //f->setInners(&fHedgeList);
                        //f->setOuter(fHedgeList.front());
                        f->setOuter(originFaceLast);                       

                        // Obstacle vertex 기준 Upper face 만들기 
                        Face* uf = new Face();
                        std::vector<HEdge*> ufHedgeList;
                        if (isUpper) {
                            upFaceFirst->setPrev(upperRight.front());
                            upperRight.front()->setNext(upFaceFirst);
                            upFaceLast->setNext(upperRight.back());
                            upperRight.back()->setPrev(upFaceLast);
                        }
                        else {
                            upFaceFirst->setPrev(upperLeft.back());
                            upperLeft.back()->setNext(upFaceFirst);
                            upFaceLast->setNext(upperLeft.front());
                            upperLeft.front()->setPrev(upFaceLast);
                        }
                        make_Hedge_list(upFaceFirst, ufHedgeList);
                        //uf->setInners(&ufHedgeList);
                        uf->setOuter(ufHedgeList.front());

                        // Obstacle vertex 기준 Lower face 만들기
                        Face* lf = new Face();
                        std::vector<HEdge*> lfHedgeList;
                        if (isUpper) {
                            loFaceFirst->setPrev(lowerRight.back());
                            lowerRight.back()->setNext(loFaceFirst);
                            loFaceLast->setNext(lowerRight.front());
                            upperRight.front()->setPrev(loFaceLast);
                        }
                        else {
                            loFaceFirst->setPrev(lowerLeft.front());
                            lowerLeft.front()->setNext(loFaceFirst);
                            loFaceLast->setNext(lowerLeft.back());
                            upperLeft.back()->setPrev(loFaceLast);
                        }

                        // debug start
                        /*
                        HEdge* debugHEdge = loFaceLast;
                        std::cout << "v: " << v->getx() << ' ' << v->gety() << std::endl;
                        for (int i = 0; i < 10; i++) {
                            std::cout << debugHEdge->getOrigin()->getx() << ' ' << debugHEdge->getOrigin()->gety() << std::endl;
                            debugHEdge = debugHEdge->getNext();
                        }
                        */
                        // debug end

                        //make_Hedge_list(upFaceFirst, ufHedgeList);
                        //std::cout << "checkpoint7-2-*-10-before_mhl\n";
                        make_Hedge_list(loFaceLast, lfHedgeList);
                        //std::cout << "checkpoint7-2-*-10-after_mhl\n";
                        //lf->setInners(&lfHedgeList);
                        lf->setOuter(lfHedgeList.front());
                        added_faces.push_back(uf);
                        added_faces.push_back(lf);
                    }
                    // Cell 하나에 obstacle vertex 하나만 있므로 break
                    
                    else if (isUpperRayIntersect) {
                        std::cout << "checkpoint7-2-*-upper_only\n";
                        // Partition cell into left cell (old cell) and right cell (new cell) 
                        if (isUpperIntersectVertex) {
                            std::cout << "checkpoint7-2-*-upper_only-vertex\n";                        
                            upFaceFirst = firstEdge;
                            upFaceLast = het->getPrev();
                            originFaceFirst = het;
                            originFaceLast = firstEdge->getPrev();

                            upFaceFirst->setPrev(upperRight.front());
                            upperRight.front()->setNext(upFaceFirst);
                            upFaceLast->setNext(upperRight.back());
                            upperRight.back()->setPrev(upFaceLast);
                            originFaceFirst->setPrev(upperLeft.back());
                            upperLeft.back()->setNext(originFaceFirst);
                            originFaceLast->setNext(upperLeft.front());
                            upperLeft.front()->setPrev(originFaceLast);
                        }
                        else {
                            std::cout << "checkpoint7-2-*-upper_only-edge\n";
                            // Split original edges                           
                            upFaceFirst = firstEdge;
                            upFaceLast = het;
                            originFaceFirst = new HEdge();
                            originFaceFirst->setOrigin(upperVertices.back());
                            originFaceFirst->setNext(het->getNext());
                            originFaceFirst->getNext()->setPrev(originFaceFirst);
                            originFaceLast = firstEdge->getPrev();

                            // Update DCEL of neighbor cell
                            HEdge* upFaceTwin = new HEdge();
                            HEdge* originFaceFirstTwin = het->getTwin();
                            upFaceTwin->setOrigin(upperVertices.back());
                            upFaceTwin->setNext(originFaceFirstTwin->getNext());
                            upFaceTwin->getNext()->setPrev(upFaceTwin);
                            upFaceTwin->setPrev(originFaceFirstTwin);
                            upFaceTwin->setTwin(upFaceLast);
                            upFaceLast->setTwin(upFaceTwin);
                            originFaceFirstTwin->setNext(upFaceTwin);
                            originFaceFirstTwin->setTwin(originFaceFirst);
                            originFaceFirst->setTwin(originFaceFirstTwin);

                            upFaceFirst->setPrev(upperRight.front());
                            upperRight.front()->setNext(upFaceFirst);
                            upFaceLast->setNext(upperRight.back());
                            upperRight.back()->setPrev(upFaceLast);
                            originFaceFirst->setPrev(upperLeft.back());
                            upperLeft.back()->setNext(originFaceFirst);
                            originFaceLast->setNext(upperLeft.front());
                            upperLeft.front()->setPrev(originFaceLast);
                        }

                        // Modify 
                        f->setOuter(originFaceLast);

                        // Make new Cell
                        Face* uf = new Face();
                        std::vector<HEdge*> ufHedgeList;
                        //std::cout << "checkpoint7-2-*-upper_only-before_mhl\n";
                        make_Hedge_list(upFaceFirst, ufHedgeList);
                        //std::cout << "checkpoint7-2-*-upper_only-after_mhl\n";
                        uf->setOuter(ufHedgeList.front());
                        added_faces.push_back(uf);
                    }

                    else if (isLowerRayIntersect) {
                        std::cout << "checkpoint7-2-*-lower_only\n";
                        // Partition cell into left cell (old cell) and right cell (new cell) 
                        if (isLowerIntersectVertex) {
                            std::cout << "checkpoint7-2-*-lower_only-vertex\n";
                            loFaceFirst = firstEdge->getPrev();
                            loFaceLast = heb;
                            originFaceFirst = heb->getPrev();
                            originFaceLast = firstEdge;

                            loFaceFirst->setNext(lowerRight.front());
                            lowerRight.front()->setPrev(loFaceFirst);
                            loFaceLast->setPrev(lowerRight.back());
                            lowerRight.back()->setNext(loFaceLast);
                            originFaceFirst->setNext(lowerLeft.back());
                            lowerLeft.back()->setPrev(originFaceFirst);
                            originFaceLast->setPrev(lowerLeft.front());
                            lowerLeft.front()->setNext(originFaceLast);
                        }
                        else {
                            std::cout << "checkpoint7-2-*-lower_only-edge\n";
                            loFaceLast = firstEdge->getPrev();
                            // Split original edges                                    
                            loFaceFirst = new HEdge();
                            originFaceLast = heb;
                            loFaceFirst->setOrigin(lowerVertices.back());
                            loFaceFirst->setNext(originFaceLast->getNext());
                            loFaceFirst->getNext()->setPrev(loFaceFirst);
                            originFaceFirst = firstEdge;

                            // Update DCEL of neighbor cell
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

                            loFaceFirst->setPrev(lowerRight.back());
                            lowerRight.back()->setNext(loFaceFirst);
                            loFaceLast->setNext(lowerRight.front());
                            lowerRight.front()->setPrev(loFaceLast);
                            originFaceFirst->setPrev(lowerLeft.front());
                            lowerLeft.front()->setNext(originFaceFirst);
                            originFaceLast->setNext(lowerLeft.back()); 
                            lowerLeft.back()->setPrev(originFaceLast);
                        }

                        // Modify 
                        f->setOuter(originFaceLast);

                        // Make new Cell
                        Face* lf = new Face();
                        std::vector<HEdge*> lfHedgeList;
                        make_Hedge_list(loFaceFirst, lfHedgeList);
                        lf->setOuter(lfHedgeList.front());
                        added_faces.push_back(lf);
                    }

                    break;
                }
            }
        }
        //std::cout << "checkpoint7-2\n";
        //std::cout << debugObs << "\n";
        std::cout << added_faces.size() << "\n";
        for (auto af : added_faces) {
            faces.push_back(af);
        }
        //std::cout << "checkpoint7-3\n";

        // Construct graph for result DCEL
        std::vector<Point> G_V;
        std::unordered_map<std::string, int> G_V_map; // vertex key, index of G_V
        std::vector<std::vector<int>> G_E;
        std::vector<int> temp_list;
        // Check time complexity of unordered map find
        for (auto f : faces) {
            //std::vector<HEdge*> hedges = f->getInnerHEdges();
            std::vector<HEdge*> hedges = f->getOutHEdges();
            for (auto he : hedges) {
                Vertex u = *he->getOrigin();
                Vertex v = *he->getNext()->getOrigin();
                // 
                if (G_V_map.find(u.getKey()) == G_V_map.end()) {
                    G_V_map[u.getKey()] = G_V.size();
                    G_V.push_back(u.getPoint());
                    G_E.push_back(temp_list);
                }
                if (G_V_map.find(v.getKey()) == G_V_map.end()) {
                    G_V_map[v.getKey()] = G_V.size();
                    G_V.push_back(v.getPoint());
                    G_E.push_back(temp_list);
                }
                G_E[G_V_map[u.getKey()]].push_back(G_V_map[v.getKey()]);
            }
        }
        //std::cout << "checkpoint7-4\n";

        std::vector<std::pair<std::string, Point>> G_V_key;
        for (auto it = G_V_map.begin(); it != G_V_map.end(); it++) {
            G_V_key.push_back(std::make_pair(it->first, G_V[it->second]));
        }
        this->subdiv = new DCEL(G_V_key,G_E);

        /*
        std::cout << this->subdiv->getFaces().size() << std::endl;
        std::vector<Face*> debugFace = this->subdiv->getFaces();
        int debugInners = 0;
        for (size_t i = 0; i < debugFace.size(); i++) {
            if (debugFace[i]->getInnerHEdges().size() != 0) std::cout << i << std::endl;
            debugInners += debugFace[i]->getInnerHEdges().size();
        }
        std::cout << debugInners << std::endl;
        */

        //std::cout << "checkpoint7-5\n";
        print_result(&G_V, &G_E, "CS_Free.txt");
        // Update edges map
        this->edge_types.clear();
        std::vector<HEdge*> edges = this->subdiv->getHedges();
        //std::vector<Vertex*> vertices = this->subdiv.getVertices();
        for (auto e : edges) {            
            std::string key = e->getKey(); // Check this code
            std::string u = e->getOrigin()->getKey();
            std::string v = e->getNext()->getOrigin()->getKey();            
            if (v.front() == 'v' || u.front() == 'v' || v.front() == 'v' || u.front() == 'v') {
                this->edge_types[key] = HE_TRP;
            }
            else {
                this->edge_types[key] = HE_OPQ;
            }            
        }
        //std::cout << "checkpoint7-5\n";
        //print_result(&G_V, &G_V_map, &G_E, "CS_Free.txt");
    }
    //std::cout << "checkpoint7-6\n";
}

CS_Free::~CS_Free(){}

DCEL* constructObsDCEL(std::vector<SimplePolygon>& obstacles, std::string key) {
    //std::vector<Point>&, std::vector<std::vector<int>>&
    // Initialize
    std::vector<Point> vertex;
    for (auto nowPolygon : obstacles) {
        std::vector<Point> nowPts = nowPolygon.getVertices();
        for (auto p : nowPts) vertex.push_back(p);
    }
    std::vector<std::vector<int>> graph(vertex.size(), std::vector<int>());

    // Adjacent list
    int startIdx = 0;
    for (auto nowPolygon : obstacles) {
        std::vector<Point> nowPts = nowPolygon.getVertices();

        if (nowPts.size() > 1) {
            graph[startIdx].push_back(startIdx + 1);
            graph[startIdx].push_back(startIdx + nowPts.size() - 1);
        }
        
        for (int i = 1; i < nowPts.size() - 1; i++) {
            graph[startIdx + i].push_back(startIdx + i - 1);
            graph[startIdx + i].push_back(startIdx + i + 1);
        }
        
        if (nowPts.size() > 2) {
            graph[startIdx + nowPts.size() - 1].push_back(startIdx + nowPts.size() - 2);
            graph[startIdx + nowPts.size() - 1].push_back(startIdx);
        }        
        
        startIdx += nowPts.size();
    }

    // To avoid duplicating the key in S
    std::vector<std::pair<std::string,Point>> labled_vertex;
    for (int i = 0; i < vertex.size(); i++) {
        labled_vertex.push_back(std::make_pair("O_" + std::to_string(i), vertex[i]));   
    }

    DCEL* ret = new DCEL(labled_vertex, graph, key);

    return ret;
}

Point verticalRayIntersection(Point origin, Edge edge) {
    Point result;
    result.setx(origin.getx());
    double slope;
    double intercept;
    Point s = edge.gets();
    Point t = edge.gett();

    // If edge if vertical
    if (std::abs(s.getx() - t.getx()) < TOLERANCE_CS_FREE) {
        if (origin.gety() < s.gety()) {
            if (s.gety() < t.gety()) {
                result.sety(s.gety());
            }
            else {
                result.sety(t.gety());
            }
        }
        else {
            if (s.gety() < t.gety()) {
                result.sety(t.gety());
            }
            else {
                result.sety(s.gety());
            }
        }
    }

    else {
        slope = (s.gety() - t.gety()) / (s.getx() - t.getx());
        intercept = s.gety() - slope * s.getx();

        result.sety(slope * result.getx() + intercept);
    }
    
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

bool isEdgeVertical(HEdge* const edge) {
    return (std::abs(edge->getEdge().gets().gety() - edge->getEdge().gett().gety()) < TOLERANCE_CS_FREE);
}

bool isEdgeHorizontal(HEdge* const edge) {
    return (std::abs(edge->getEdge().gets().getx() - edge->getEdge().gett().getx()) < TOLERANCE_CS_FREE);
}

void point_location_queries(std::vector<Vertex*>* s_vertices, DCEL* obstacles, std::vector<PLQ>& results) {
    Location loc(obstacles);

    for (size_t i = 0; i < results.size(); i++) {
        auto v = (*s_vertices)[i];
        Point temp = v->getPoint();

        results[i].answer[0] = loc.ortho_ray(&temp, Location::N);
        results[i].answer[1] = loc.ortho_ray(&temp, Location::S);
        results[i].answer[2] = loc.ortho_ray(&temp, Location::E);
        results[i].answer[3] = loc.ortho_ray(&temp, Location::W);
    }
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

bool comparePoint1(std::pair<Point, int> a, std::pair<Point, int> b) {
    return a.first.gety() < b.first.gety();
}

bool comparePoint2(std::pair<Point, int> a, std::pair<Point, int> b) {
    return a.first.gety() > b.first.gety();
}

bool comparePoint3(std::pair<Point, int> a, std::pair<Point, int> b) {
    return a.first.getx() < b.first.getx();
}

bool comparePoint4(std::pair<Point, int> a, std::pair<Point, int> b) {
    return a.first.getx() > b.first.getx();
}


void print_result(const std::vector<Point>* const G_V, const std::unordered_map<std::string, int>* const G_V_map, const std::vector<std::vector<int>>* const G_E, const std::string filename) {
    std::ofstream ofs;
    ofs.open(filename);

    ofs << G_V->size() << std::endl;
    for (const Point nowP : (*G_V)) {
        ofs << nowP.getx() << ' ' << nowP.gety() << std::endl;
    }

    int numE = 0;
    for (int i = 0; i < G_E->size(); i++) {
        numE += (*G_E)[i].size();
    }
    ofs << numE << endl;
    for (int i = 0; i < G_E->size(); i++) {
        for (int j = 0; j < (*G_E)[i].size(); j++) {
            ofs << i << ' ' << (*G_E)[i][j] << std::endl;
        }
    }

    ofs.close();
}

void print_result(const std::vector<Point>* const G_V, const std::vector<std::vector<int>>* const G_E, const std::string filename) {
    std::ofstream ofs;
    ofs.open(filename);

    ofs << G_V->size() << std::endl;
    for (const Point nowP : (*G_V)) {
        ofs << nowP.getx() << ' ' << nowP.gety() << std::endl;
    }

    int numE = 0;
    for (int i = 0; i < G_E->size(); i++) {
        numE += (*G_E)[i].size();
    }
    ofs << numE << endl;
    for (int i = 0; i < G_E->size(); i++) {
        for (int j = 0; j < (*G_E)[i].size(); j++) {
            ofs << i << ' ' << (*G_E)[i][j] << std::endl;
        }
    }

    ofs.close();
}

void print_Neighbor(const Neighbor a) {
    bool isNull[4] = { true, true, true, true };
    bool isChanged[4] = { false, false, false, false };
    Point neighbor[4];
    int v_idx[4] = { -1, -1, -1, -1 };

    std::cout << a.isNull[0] << ' ' << a.isNull[1] << ' ' << a.isNull[2] << ' ' << a.isNull[3] << std::endl;
    std::cout << a.isChanged[0] << ' ' << a.isChanged[1] << ' ' << a.isChanged[2] << ' ' << a.isChanged[3] << std::endl;
    std::cout << a.v_idx[0] << ' ' << a.v_idx[1] << ' ' << a.v_idx[2] << ' ' << a.v_idx[3] << std::endl;
}

bool sort_by_idx(std::pair<Point, int> a, std::pair<Point, int> b) {
    return a.second < b.second;
}

bool isPointInner(Point p, HEdge* prev, HEdge* now) {
    double anglePrevPoint = getAngle(prev->getOrigin()->getPoint(),now->getOrigin()->getPoint(),p);
    double anglePrevNow = getAngle(prev->getOrigin()->getPoint(), now->getOrigin()->getPoint(), now->getNext()->getOrigin()->getPoint());
    if (anglePrevPoint < anglePrevNow) return true;
    else return false;
}

double getAngle(Point a, Point b, Point c) {
    int direction = getDirection(a, b, c);
    if (direction == 0) return 180.0;

    double SmallAngle;
    double aa, bb, cc;
    double ip;

    aa = std::sqrt(std::pow(a.getx() - c.getx(), 2) + std::pow(a.gety() - c.gety(), 2));
    bb = std::sqrt(std::pow(a.getx() - b.getx(), 2) + std::pow(a.gety() - b.gety(), 2));
    cc = std::sqrt(std::pow(b.getx() - c.getx(), 2) + std::pow(b.gety() - c.gety(), 2));

    ip = (std::pow(bb, 2) + std::pow(cc, 2) - std::pow(aa, 2)) / (2 * bb * cc);
    SmallAngle = std::acos(ip);
    SmallAngle = SmallAngle * (180 / PI_CS_FREE);

    if (direction == -1) {
        return SmallAngle;
    }
    else {
        return 360.0 - SmallAngle;
    }
}

int getDirection(Point a, Point b, Point c) {
    int ccw = (b.getx() - a.getx()) * (c.gety() - a.gety()) - (c.getx() - a.getx()) * (b.gety() - a.gety());
    if (ccw < 0) return -1;
    else if (ccw == 0) return 0;
    else return 1;
}