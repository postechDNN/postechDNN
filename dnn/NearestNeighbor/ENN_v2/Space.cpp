#include "Space.h"
#include "AVLTreeList.h"
#include <limits>
#include <queue>
#include <tuple>
#include <functional>
#include <algorithm>
#include <map>

#define M_PI		3.14159265358979323846

//Constructor
VisGraph::VisGraph(const vector<Point>& _srcs, const vector<SimplePolygon>& _obstacles) {
    set_Space(_srcs, _obstacles);
}

//Destructor
VisGraph::~VisGraph() {
}

//Reconstruct the data structure when the input is modified (addition or deletion of a site or polygon)
void  VisGraph::set_Space(const vector<Point>& _srcs, const vector<SimplePolygon>& _obstacles) {
    this->srcs.clear();
    this->obstacles.clear();
    this->edges.clear();
    this->vertices.clear();
    for (int i = 0; i < _srcs.size(); i++) {
        srcs.push_back(_srcs[i]);
        vertices.push_back(_srcs[i]);
        srcs[i].index = i;
        vertices[i].index = i;
    }
    for (int i = 0; i < _obstacles.size(); i++) {
        obstacles.push_back(_obstacles[i]);
        obstacles[i].index = i;

        for (auto E : obstacles[i].getEdges()) {
            Edge_S T = E;
            T.poly = i;
            edges.push_back(T);
        }

        for (auto V : obstacles[i].getVertices()) {
            Point_S T = V;
            T.poly = i;
            T.index = vertices.size();
            for (int j = 0; j < obstacles[i].getEdges().size(); j++) {
                if (V == obstacles[i].getEdges()[j].gets()) { T.edge_s = j; }
                else if (V == obstacles[i].getEdges()[j].gett()) { T.edge_t = j; }
            }
            vertices.push_back(T);
        }
    }
    for (int i = 0; i < vertices.size(); i++) {
        vector<pair<int, double>>* temp = new vector<pair<int, double>>;
        adj_list.push_back(*temp);
    }
    visibility_graph();
    Dijkstra();
}

//Compare two points in angular order with respect to the origin
struct cmp {
    bool operator()(tuple<double, double, Point_S> a, tuple<double, double, Point_S> b) {
        if (abs(get<0>(a) - get<0>(b)) < ERR)
            return get<1>(a) > get<1>(b);
        else
            return get<0>(a) > get<0>(b);
    }
};

//Build the visibility graph with the sites and obstacles
void VisGraph::visibility_graph() {
    for (int i = 0; i < vertices.size(); i++) {
        priority_queue<tuple<double, double, Point_S>, vector<tuple<double, double, Point_S>>, cmp> P; // Event list
        for (int j = 0; j < vertices.size(); j++) {
            if (i == j) continue;
            double alpha = atan2(vertices[j].gety() - vertices[i].gety(), vertices[j].getx() - vertices[i].getx());
            if (alpha < 0.)alpha += 2 * M_PI;
            double dist = vertices[i].distance(vertices[j]);
            P.emplace(alpha, dist, vertices[j]);
        }

        AVLTreeList S; // active edge list
        AVLTreeList Init;

        // Set active edge for Horizontal line
        Point x(vertices[i].getx() + 1 / EPS, vertices[i].gety());
        Edge temp(vertices[i], x);
        for (auto edge : edges) {
            if (edge.crossing(temp, false) != nullptr) {
                Point T = edge.crossing(temp, false)->gets();
                S.Insert(edge, vertices[i], x);
                Init.Insert(edge, vertices[i], x);
            }
        }

        while (!P.empty())
        {
            tuple<double, double, Point_S> event = P.top();
            P.pop();
            if (get<2>(event) == vertices[i]) continue;

            // two vertices are in the same polygon
            if (vertices[i].poly >= 0 && vertices[i].poly == get<2>(event).poly) {
                Point p(vertices[i].getx() + (get<2>(event).getx() - vertices[i].getx()) * EPS, vertices[i].gety() + (get<2>(event).gety() - vertices[i].gety()) * EPS);
                // two vertices are in the same edge
                if (vertices[i].edge_s == get<2>(event).edge_t || vertices[i].edge_t == get<2>(event).edge_s) {
                    adj_list[i].push_back({ get<2>(event).index , vertices[i].distance(get<2>(event)) });
                }
                else if (!obstacles[vertices[i].poly].inPolygon(p)) {
                    Edge temp(vertices[i], get<2>(event));
                    // event vertex is visible from p
                    if (S.Is_empty() || S.Search().crossing(temp, true) == nullptr || S.Search().crossing(temp, true)->gets() == get<2>(event)) {
                        adj_list[i].push_back({ get<2>(event).index , vertices[i].distance(get<2>(event)) });
                    }
                }
            }
            else {
                Edge temp(vertices[i], get<2>(event));
                // event vertex is visible from p
                if (S.Is_empty() || S.Search().crossing(temp, true) == nullptr || S.Search().crossing(temp, true)->gets() == get<2>(event)) {
                    adj_list[i].push_back({ get<2>(event).index , vertices[i].distance(get<2>(event)) });
                }
            }

            // event vertex is an edge
            if (get<2>(event).poly >= 0) {

                Edge E1 = obstacles[get<2>(event).poly].getEdges()[get<2>(event).edge_s];
                double alpha1 = atan2(E1.gets().gety() - vertices[i].gety(), E1.gets().getx() - vertices[i].getx());
                if (alpha1 < 0.)alpha1 += 2 * M_PI;
                double alpha2 = atan2(E1.gett().gety() - vertices[i].gety(), E1.gett().getx() - vertices[i].getx());
                if (alpha2 < 0.)alpha2 += 2 * M_PI;

                Edge E2 = obstacles[get<2>(event).poly].getEdges()[get<2>(event).edge_t];
                double alpha3 = atan2(E2.gets().gety() - vertices[i].gety(), E2.gets().getx() - vertices[i].getx());
                if (alpha3 < 0.)alpha3 += 2 * M_PI;
                double alpha4 = atan2(E2.gett().gety() - vertices[i].gety(), E2.gett().getx() - vertices[i].getx());
                if (alpha4 < 0.)alpha4 += 2 * M_PI;

                bool check1 = Init.Search(E1, vertices[i]);
                bool check2 = Init.Search(E2, vertices[i]);
                if (((check1 && alpha1 + EPS < alpha2) || (!check1 && alpha1 > alpha2 + EPS)) && alpha1 > EPS && alpha2 > EPS) {
                    S.Delete(E1, vertices[i], get<2>(event)); // E1 Edge ends
                }
                if (((check2 && alpha3 > alpha4 + EPS) || (!check2 && alpha3 + EPS < alpha4)) && alpha3 > EPS && alpha4 > EPS) {
                    S.Delete(E2, vertices[i], get<2>(event)); // E2 Edge ends
                }

                if (((!check1 && alpha1 + EPS < alpha2) || (check1 && alpha1 > alpha2 + EPS)) && alpha1 > EPS && alpha2 > EPS) {
                    S.Insert(E1, vertices[i], get<2>(event)); // E1 Edge ends
                }
                if (((!check2 && alpha3 > alpha4 + EPS) || (check2 && alpha3 + EPS < alpha4)) && alpha3 > EPS && alpha4 > EPS) {
                    S.Insert(E2, vertices[i], get<2>(event)); // E2 Edge ends
                }
            }
        }
    }
};


//Add a polygon and modify the visibility graph
void VisGraph::add_Polygon(SimplePolygon poly) {
    this->obstacles.push_back(poly);
    std::vector<Point> _srcs;
    std::vector<SimplePolygon> _obs;
    for (size_t i = 0; i < this->srcs.size(); i++) {
        _srcs.push_back(this->srcs[i]);
    }
    for (size_t i = 0; i < this->obstacles.size(); i++) {
        _obs.push_back(this->obstacles[i]);
    }
    this->set_Space(_srcs, _obs);
}


//Delete a polygon and modify the visibility graph
void VisGraph::del_Polygon(int i) {
    this->obstacles.erase(obstacles.begin() + i);
    std::vector<Point> _srcs;
    std::vector<SimplePolygon> _obs;
    for (size_t i = 0; i < this->srcs.size(); i++) {
        _srcs.push_back(this->srcs[i]);
    }
    for (size_t i = 0; i < this->obstacles.size(); i++) {
        _obs.push_back(this->obstacles[i]);
    }
    this->set_Space(_srcs, _obs);
}


//Add a site and modify the visibility graph
void VisGraph::add_vert(Point p) {
    this->srcs.push_back(p);
    std::vector<Point> _srcs;
    std::vector<SimplePolygon> _obs;
    for (size_t i = 0; i < this->srcs.size(); i++) {
        _srcs.push_back(this->srcs[i]);
    }
    for (size_t i = 0; i < this->obstacles.size(); i++) {
        _obs.push_back(this->obstacles[i]);
    }
    this->set_Space(_srcs, _obs);
}


//Delete a site and modify the visibility graph
void VisGraph::del_vert(int i) {
    this->srcs.erase(srcs.begin() + i);
    std::vector<Point> _srcs;
    std::vector<SimplePolygon> _obs;
    for (size_t i = 0; i < this->srcs.size(); i++) {
        _srcs.push_back(this->srcs[i]);
    }
    for (size_t i = 0; i < this->obstacles.size(); i++) {
        _obs.push_back(this->obstacles[i]);
    }
    this->set_Space(_srcs, _obs);
}


//Build the nearest neighbor data structure using Dijkstra algorithm
void VisGraph::Dijkstra() {
    //Modify code using fibonacci heap
    dists.assign(vertices.size(), -std::numeric_limits<double>::lowest());
    visited.assign(vertices.size(), false);
    near_src.assign(vertices.size(), -1);
    std::priority_queue<std::tuple<double, int, int>, std::vector<std::tuple<double, int, int>>, greater<std::tuple<double, int, int>>> que = {};
    for (int j = 0; j < srcs.size(); j++) {
        que.emplace(0, j, j);
        dists[j] = 0;
        near_src[j] = j;
    }
    while (!que.empty()) {
        tuple<double, int, int> temp = que.top();
        que.pop();
        if (visited[get<1>(temp)] == true) continue;
        visited[std::get<1>(temp)] = true;
        for (auto& v : this->adj_list[std::get<1>(temp)]) {
            if (dists[std::get<1>(temp)] + v.second < dists[v.first]) {
                dists[v.first] = dists[std::get<1>(temp)] + v.second;
                near_src[v.first] = get<2>(temp);
                que.emplace(dists[v.first], v.first, near_src[v.first]);
            }
        }
    }
}


//Find the nearest site from the query point and return the coordinate, index, and distance of the site
tuple<Point, int, double> VisGraph::query(Point query) {
    Point Near;
    int near_idx = -1;
    double dist = INFINITY;

    priority_queue<tuple<double, double, Point_S>, vector<tuple<double, double, Point_S>>, cmp> P; // Event list
    for (int j = 0; j < vertices.size(); j++) {
        double alpha = atan2(vertices[j].gety() - query.gety(), vertices[j].getx() - query.getx());
        double dist2 = query.distance(vertices[j]);
        if (alpha < 0.)alpha += 2 * M_PI;
        P.emplace(alpha, dist2, vertices[j]);
    }

    AVLTreeList S; // active edge list
    AVLTreeList Init;

    // Set active edge for Horizontal line
    Point x(query.getx() + 1 / EPS, query.gety());
    Edge temp(query, x);
    for (auto edge : edges) {
        if (edge.crossing(temp, false) != nullptr) {
            Point T = edge.crossing(temp, false)->gets();
            S.Insert(edge, query, x);
            Init.Insert(edge, query, x);
        }
    }

    while (!P.empty()) {
        tuple<double, double, Point_S> event = P.top();
        P.pop();

        Edge temp(query, get<2>(event));
        // event vertex is visible from p
        if (S.Is_empty() || S.Search().crossing(temp, true) == nullptr || S.Search().crossing(temp, true)->gets() == get<2>(event)) {
            double dist_temp = query.distance(get<2>(event));
            // event vertex is an edge
            if (get<2>(event).poly >= 0 && dist_temp + dists[get<2>(event).index] < dist) {
                Near = vertices[near_src[get<2>(event).index]];
                near_idx = get<2>(event).index;
                dist = dist_temp + dists[get<2>(event).index];
            }
            else if (get<2>(event).poly < 0 && dist_temp < dist) {
                Near = get<2>(event);
                near_idx = get<2>(event).index;
                dist = dist_temp;
            }
        }

        // event vertex is an edge
        if (get<2>(event).poly >= 0) {

            Edge E1 = obstacles[get<2>(event).poly].getEdges()[get<2>(event).edge_s];
            double alpha1 = atan2(E1.gets().gety() - query.gety(), E1.gets().getx() - query.getx());
            if (alpha1 < 0.)alpha1 += 2 * M_PI;
            double alpha2 = atan2(E1.gett().gety() - query.gety(), E1.gett().getx() - query.getx());
            if (alpha2 < 0.)alpha2 += 2 * M_PI;

            Edge E2 = obstacles[get<2>(event).poly].getEdges()[get<2>(event).edge_t];
            double alpha3 = atan2(E2.gets().gety() - query.gety(), E2.gets().getx() - query.getx());
            if (alpha3 < 0.)alpha3 += 2 * M_PI;
            double alpha4 = atan2(E2.gett().gety() - query.gety(), E2.gett().getx() - query.getx());
            if (alpha4 < 0.)alpha4 += 2 * M_PI;

            bool check1 = Init.Search(E1, query);
            bool check2 = Init.Search(E2, query);
            if (((check1 && alpha1 + EPS < alpha2) || (!check1 && alpha1 > alpha2 + EPS)) && alpha1 > EPS && alpha2 > EPS) {
                S.Delete(E1, query, get<2>(event)); // E1 Edge ends
            }
            if (((check2 && alpha3 > alpha4 + EPS) || (!check2 && alpha3 + EPS < alpha4)) && alpha3 > EPS && alpha4 > EPS) {
                S.Delete(E2, query, get<2>(event)); // E2 Edge ends
            }

            if (((!check1 && alpha1 + EPS < alpha2) || (check1 && alpha1 > alpha2 + EPS)) && alpha1 > EPS && alpha2 > EPS) {
                S.Insert(E1, query, get<2>(event)); // E1 Edge ends
            }
            if (((!check2 && alpha3 > alpha4 + EPS) || (check2 && alpha3 + EPS < alpha4)) && alpha3 > EPS && alpha4 > EPS) {
                S.Insert(E2, query, get<2>(event)); // E2 Edge ends
            }
        }
    }

    return { Near, near_idx, dist };
}