#include "Space.h"
#include <limits>
#include <queue>

Arrangement::Arrangement(int num_srcs, vector<Point> vertices):DCEL() {
    double x_max = 0;
    double y_max = 0;
    for (size_t i = 0; i < vertices.size(); i++) {
        for (size_t j = 0; j < i; j++) {
            if (abs(vertices[i].getx() - vertices[j].getx()) < ERR) continue;
            double x_temp = (vertices[j].gety() - vertices[i].gety()) / (vertices[j].getx() - vertices[i].getx());
            double y_temp = (vertices[j].getx() * vertices[i].gety() - vertices[i].getx()* vertices[j].gety()) / (vertices[j].getx() - vertices[i].getx());
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
                } while (s1*s2 >= 0);
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
    Vertex* v0 = new Vertex(-x_max,-y_max);
    Vertex* v1 = new Vertex(x_max, -y_max);
    Vertex* v2 = new Vertex(x_max, y_max);
    Vertex* v3 = new Vertex(-x_max, y_max);
    std::vector<Vertex*> rect_verts{v0,v1,v2,v3};
    this->setVertices(rect_verts);
    std::vector<HEdge*> rect_hedges;
    for (size_t i = 0; i < rect_verts.size(); i++) {
        rect_hedges.push_back(new HEdge(rect_verts[i], rect_verts[(i + 1)% rect_verts.size()]));
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

Space::Space(vector<Point> _srcs, vector<SimplePolygon> _obstacles) {
    srcs = _srcs;
    obstacles = _obstacles;
    
}

Space::~Space(){
}

void  Space::set_Space(vector<Point>, vector<SimplePolygon>) {

}






void Space::visibility_graph() {
    vector<Point> vertices(this->srcs);
    for (auto& o : obstacles) {
        vertices.insert(vertices.end(), o.getVertices().begin(), o.getVertices().end());
    }
    this->adj_list.clear();
    for (long long int i = 0; i < size(vertices); i++) {
        vector<pair<long long, double>> temp;
        for (long long j = 0; j < size(vertices); j++) {
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
    visited.assign(size(vertices), false);
};


void Space::add_Polygon(SimplePolygon) {

}

void Space::del_Polygon(int) {

}
void Space::add_vert(Point) {

}
void Space::del_vert(int) {

}
void Space::Dijkstra(Point query) {
    dists.assign(size(dists), std::numeric_limits<double>::max());
    visited.assign(size(visited), false);
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, greater<std::pair<double, int>>> que = {};
    for (long long j = 0; j < size(vertices); j++) {
        bool intersect = false;
        for (auto& o : obstacles) {
            auto edges = o.getEdges();
            for (auto& e : edges) {
                Edge e1 = Edge(query, vertices[j]);
                if (e.crossing(e1, false) == nullptr || e == e1) {
                    dists[j] = Vector(vertices[j], query).norm();
                    que.emplace(dists[j], j);
                }
            }
        }
    }
    while (!que.empty()) {
        if (visited[que.top().second] == true) {
            que.pop();
            continue;
        }
        std::pair<double, int> temp = que.top();
        que.pop();
        visited[temp.second] = true;
        for (auto& v : this->adj_list[temp.second]) {
            if (dists[temp.second] + v.second < dists[v.first]) {
                dists[v.first] = dists[temp.second] + v.second;
                que.emplace(dists[v.first],v.first);
            }
        }
    }
}
void Space::print_knn(int k) {
    k = min(k, int(size(srcs)));

}

