#include "pch.h"
#include "Space.h"
#include "AVLTreeList.h"
#include <limits>
#include <queue>
#include <tuple>
#include <functional>
#include <algorithm>
#include <map>

#define M_PI		3.14159265358979323846
#define EPS 0.0001

Arrangement::Arrangement(const vector<Point>& vertices) :DCEL() {
    double x_max = 0;
    double y_max = 0;
    for (size_t i = 0; i < vertices.size(); i++) {
        for (size_t j = 0; j < i; j++) {
            if (abs(vertices[i].getx() - vertices[j].getx()) < ERR) continue;
            double x_temp = (vertices[j].gety() - vertices[i].gety()) / (vertices[j].getx() - vertices[i].getx());
            double y_temp = (vertices[j].getx() * vertices[i].gety() - vertices[i].getx() * vertices[j].gety()) / (vertices[j].getx() - vertices[i].getx());
            if (x_max < abs(x_temp))
                x_max = abs(x_temp);
            if (y_max < abs(y_temp))
                y_max = abs(y_temp);
        }
    }
    x_max += 1;
    y_max += 1;
    make_Rectangle(x_max, y_max);
    for (size_t i = 0; i < vertices.size(); i++) {
        double a = vertices[i].getx();
        double b = -vertices[i].gety();
        Edge e(Point(-2 * x_max, -2 * x_max * a + b), Point(2 * x_max, 2 * x_max * a + b));
        Face* temp_f = this->faces["f_1"];
        //Get start point
        HEdge* temp_he = temp_f->getInners()[0];
        if (a < 0) temp_he = temp_he->getNext();
        Edge* p = nullptr;
        do {
            temp_he = temp_he->getNext();
            Edge temp_e(*(temp_he->getOrigin()), *(temp_he->getTwin()->getOrigin()));
            p = e.crossing(temp_e, true);
        } while (p == nullptr || !(p->gets() == p->gett()));
        if (p->gets() == temp_he->getOrigin()->getPoint()) {
            temp_he = temp_he->getPrev()->getTwin();
            start_verts.push_back(temp_he->getOrigin());
        }
        else if (p->gets() == temp_he->getTwin()->getOrigin()->getPoint()) {
            temp_he = temp_he->getTwin();
            start_verts.push_back(temp_he->getTwin()->getOrigin());
        }
        else {
            Vertex* n_v = new Vertex(p->gets());
            start_verts.push_back(n_v);
            this->vertices[n_v->getKey()] = n_v;
            HEdge* n_he = new HEdge(n_v, temp_he->getTwin()->getOrigin());
            this->hedges[n_he->getKey()] = n_he;
            this->hedges[n_he->getTwin()->getKey()] = n_he->getTwin();
            n_v->setIncidentEdge(n_he);
            n_he->setIncidentFace(temp_f);
            n_he->getTwin()->setIncidentFace(temp_he->getTwin()->getIncidentFace());
            n_he->setPrev(temp_he);
            n_he->setNext(temp_he->getNext());
            temp_he->getNext()->setPrev(n_he);
            temp_he->setNext(n_he);

            temp_he->getTwin()->setOrigin(n_v);
            n_he->getTwin()->setNext(temp_he->getTwin());
            n_he->getTwin()->setPrev(temp_he->getTwin()->getPrev());
            temp_he->getTwin()->getPrev()->setNext(n_he->getTwin());
            temp_he->getTwin()->setPrev(n_he->getTwin());

            temp_he = temp_he->getTwin();
        }
        temp_f = temp_he->getIncidentFace();
        while (temp_f->getKey() != "f_1") {
            Edge* next_p = nullptr;
            HEdge* next_he = temp_he;
            Face* new_f = new Face();
            this->faces[new_f->getKey()] = new_f;
            new_f->setOuter(next_he);
            next_he->setIncidentFace(new_f);
            do {
                next_he = next_he->getNext();
                next_he->setIncidentFace(new_f);
                Edge next_e(*(next_he->getOrigin()), *(next_he->getTwin()->getOrigin()));
                next_p = e.crossing(next_e, true);
            } while (next_p == nullptr || !(next_p->gets() == next_p->gett()));
            Vertex* new_v = nullptr;
            if (next_p->gets() == next_he->getTwin()->getOrigin()->getPoint()) {
                new_v = next_he->getTwin()->getOrigin();
            }
            else {
                new_v = new Vertex(next_p->gets());
                this->vertices[new_v->getKey()] = new_v;
            }
            HEdge* new_he = new HEdge(temp_he->getOrigin(), new_v);
            this->he_ver_map[new_he->getKey()] = i;
            this->he_ver_map[new_he->getTwin()->getKey()] = i;
            this->hedges[new_he->getKey()] = new_he;
            this->hedges[new_he->getTwin()->getKey()] = new_he->getTwin();
            new_he->setIncidentFace(temp_f);
            new_he->getTwin()->setIncidentFace(new_f);
            new_v->setIncidentEdge(new_he->getTwin());
            temp_he->getPrev()->setNext(new_he);
            new_he->setPrev(temp_he->getPrev());
            temp_he->setPrev(new_he->getTwin());
            new_he->getTwin()->setNext(temp_he);

            p = next_p;
            if (p->gets() == next_he->getTwin()->getOrigin()->getPoint()) {
                next_he->getNext()->setPrev(new_he);
                new_he->setNext(next_he->getNext());
                new_he->getTwin()->setPrev(next_he->getTwin());
                next_he->setNext(new_he->getTwin());
                //Find an incident Hedge incident to the next face
                Vector v0(new_he->getOrigin()->getPoint(), new_he->getTwin()->getOrigin()->getPoint());
                double s1 = 0;
                double s2 = 0;
                do {
                    temp_he = next_he->getTwin();
                    s1 = v0.crossProduct(Vector(temp_he->getOrigin()->getPoint(), temp_he->getTwin()->getOrigin()->getPoint()));
                    s2 = v0.crossProduct(Vector(temp_he->getOrigin()->getPoint(), temp_he->getPrev()->getOrigin()->getPoint()));
                    next_he = temp_he->getPrev();
                } while (s1 * s2 >= 0);
            }
            else {
                HEdge* new_he2 = new HEdge(new_v, next_he->getTwin()->getOrigin());
                this->hedges[new_he2->getKey()] = new_he2;
                this->hedges[new_he2->getTwin()->getKey()] = new_he2->getTwin();
                new_he2->setIncidentFace(temp_f);
                new_he2->getTwin()->setIncidentFace(next_he->getTwin()->getIncidentFace());

                new_he2->setPrev(next_he);
                new_he2->setNext(next_he->getNext());
                next_he->getNext()->setPrev(new_he2);
                next_he->setNext(new_he2);

                next_he->getTwin()->setOrigin(new_v);
                new_he2->getTwin()->setNext(next_he->getTwin());
                new_he2->getTwin()->setPrev(next_he->getTwin()->getPrev());
                next_he->getTwin()->getPrev()->setNext(new_he2->getTwin());
                next_he->getTwin()->setPrev(new_he2->getTwin());

                temp_he = next_he->getTwin();
            }
            temp_f = temp_he->getIncidentFace();
        }

    }

}

void Arrangement::make_Rectangle(double x_max, double y_max) {
    Vertex* v0 = new Vertex(-x_max, -y_max);
    Vertex* v1 = new Vertex(x_max, -y_max);
    Vertex* v2 = new Vertex(x_max, y_max);
    Vertex* v3 = new Vertex(-x_max, y_max);
    std::vector<Vertex*> rect_verts{ v0,v1,v2,v3 };
    this->setVertices(rect_verts);
    std::vector<HEdge*> rect_hedges;
    for (size_t i = 0; i < rect_verts.size(); i++) {
        rect_hedges.push_back(new HEdge(rect_verts[i], rect_verts[(i + 1) % rect_verts.size()]));
    }
    for (size_t i = 0; i < rect_verts.size(); i++) {
        rect_hedges[i]->setNext(rect_hedges[(i + 1) % rect_verts.size()]);
        rect_hedges[i]->setPrev(rect_hedges[(i + rect_verts.size() - 1) % rect_verts.size()]);
        rect_hedges[i]->getTwin()->setNext(rect_hedges[(i + rect_verts.size() - 1) % rect_verts.size()]->getTwin());
        rect_hedges[i]->getTwin()->setPrev(rect_hedges[(i + 1) % rect_verts.size()]->getTwin());
        rect_verts[i]->setIncidentEdge(rect_hedges[i]);
        rect_hedges.push_back(rect_hedges[i]->getTwin());
    }
    this->setHedges(rect_hedges);
    //TODO:set informations for two faces
    std::vector<HEdge*> outer_edge{ rect_hedges[0]->getTwin() };
    this->getFace("f_1")->setInners(&outer_edge);
    Face* f = new Face();
    this->faces[f->getKey()] = f;
    f->setOuter(rect_hedges[0]);
    for (size_t i = 0; i < rect_verts.size(); i++) {
        rect_hedges[i]->setIncidentFace(f);
        rect_hedges[i]->getTwin()->setIncidentFace(this->faces["f_1"]);
    }
}


VisGraph::VisGraph(const vector<Point>& _srcs, const vector<SimplePolygon>& _obstacles) {
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
    set_Space(_srcs, _obstacles);
}

VisGraph::~VisGraph() {
}

void  VisGraph::set_Space(const vector<Point>& _srcs, const vector<SimplePolygon>& _obstacles) {

    //arr = Arrangement(vertices);
    visibility_graph();
    Dijkstra();
}


struct cmp {
    bool operator()(tuple<double,double, Point_S> a, tuple<double, double, Point_S> b) {
        if (abs(get<0>(a) - get<0>(b)) < ERR) 
            return get<1>(a) > get<1>(b);
        else 
            return get<0>(a) > get<0>(b); 
        
    }
};

void VisGraph::visibility_graph() {
    for (int i = 0; i < vertices.size(); i++) {

        priority_queue<tuple<double, double, Point_S>, vector<tuple<double, double, Point_S>>, cmp> P;
        //map<double, Point_S> P; // Event list
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
                    if (S.Is_empty() || S.Search().crossing(temp, true) == nullptr || S.Search().crossing(temp, true)->gets() == get<2>(event)) {//S.Search().crossing(temp, false) == nullptr
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
    //Need to Implement using arrangement
    /*
    vector<Point> vertices(this->srcs);
    for (auto& o : obstacles) {
        vertices.insert(vertices.end(), o.getVertices().begin(), o.getVertices().end());
    }
    this->adj_list.clear();
    for (int i = 0; i < size(vertices); i++) {
        vector<pair<int, double>> temp;
        for (int j = 0; j < size(vertices); j++) {
            bool intersect = false;
            for (auto& o : obstacles) {
                auto edges = o.getEdges();
                for (auto& e : edges) {
                    Edge e1 = Edge(vertices[i], vertices[j]);
                    if (e.crossing(e1, false) == nullptr || e == e1)
                        temp.push_back(make_pair(j, Vector(vertices[j], vertices[i]).norm()));
                }
            }
        }
        this->adj_list.push_back(temp);
    }
    dists.assign(size(vertices), std::numeric_limits<double>::max());
    visited.assign(size(vertices), false);*/
};


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

void VisGraph::Dijkstra() {
    //Modify code using fibonacci heap
    dists.assign(vertices.size(), - std::numeric_limits<double>::lowest());
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
        //near_src[get<1>(temp)] = get<2>(temp);
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
tuple<Point, int, double> VisGraph::query(Point query) {
    Point Near;
    int near_idx = -1;
    double dist = INFINITY;

    priority_queue<tuple<double, double, Point_S>, vector<tuple<double, double, Point_S>>, cmp> P;
    //map<double, Point_S> P; // Event list
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