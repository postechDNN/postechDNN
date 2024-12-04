
#include "Space.h"
#include <limits>
#include <queue>

Arrangement::Arrangement(int num_srcs, vector<Point> vertices):DCEL("outer") {
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
    //TODO: add dual lines incrementally (p,q)-> y=px-q
    for (size_t i = 0; i < vertices.size(); i++) {
        double a = vertices[i].getx();
        double b = -vertices[i].gety();
        Edge e(Point(-2 * x_max, -2 * x_max * a + b), Point(2 * x_max, 2 * x_max * a + b));
        Face* temp_f = this->faces["outer"];
        //Get start point
        HEdge* temp_he;
        while (temp_f->getKey() != "outer") {

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
        rect_hedges.push_back(rect_hedges[i]->getTwin());
    }
    this->setHedges(rect_hedges);
    //TODO:set informations for two faces
    std::vector<HEdge*> outer_edge{ rect_hedges[0]->getTwin() };
    this->getFace("outer")->setInners(&outer_edge);
    Face* f = new Face();
    this->faces[f->getKey()] = f;
    f->setOuter(rect_hedges[0]);
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

