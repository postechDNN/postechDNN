
#include "Space.h"
#include <limits>
#include <queue>

Space::Space(vector<Point> _srcs, vector<SimplePolygon> _obstacles) {
    srcs = _srcs;
    obstacles = _obstacles;
    
}
/*


Space::Space(const Space& p)
{
    this->d=p.d;
    this->vertices = p.vert;
    this->Boxes = p.boxes;
    cal_rmin();
    this->rmin = 0.1;
    gen_SteinerPoint();
}


*/

Space::~Space(){
}

void  Space::set_Space(int, vector<Point>, vector<SimplePolygon>) {

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


void Space::del_Box(int) {

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

