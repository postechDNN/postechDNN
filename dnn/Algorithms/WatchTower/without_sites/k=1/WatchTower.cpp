#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include "Viewpoint.h"
#define ERR 1e-6
#define MAX_X 1e+10

Edge getExtension(Edge& e, bool max_x) {
    double slope = e.getSlope();
    double x = max_x ? MAX_X : -MAX_X;
    double y = slope * x - slope * e.gets().getx() + e.gets().gety();
    return max_x ? Edge(e.gets(), Point(x, y)) : Edge(Point(x, y), e.gett());
}

double det(double a, double b, double c, double d) { return a * d - b * c; }

bool Intersection(Point* intersect, Point p1, Point p2, Point p3, Point p4) {
    double a = p1.gety() - p2.gety();
    double b = p2.getx() - p1.getx();
    double c = p3.gety() - p4.gety();
    double d = p4.getx() - p3.getx();
    double e = p1.gety() * (p2.getx() - p1.getx()) - p1.getx() * (p2.gety() - p1.gety());
    double f = p3.gety() * (p4.getx() - p3.getx()) - p3.getx() * (p4.gety() - p3.gety());
    double determinant = det(a, b, c, d);

    if (abs(determinant) <= ERR)
        return false;
    double x1 = (d * e - b * f) / determinant;
    double x2 = (a * f - c * e) / determinant;
    *intersect = Point(x1, x2);
    return true;
}

void calcHalfPlane(std::vector<Edge>& plane, std::vector<Edge>& slope) {
    Edge top, temp;
    Point i_p;
    std::vector<Edge>::iterator iter = slope.begin();
    bool is_pos = iter->gets().getx() - (iter + 1)->gets().getx() < ERR;
    while (iter != slope.end()) {
        if (plane.empty()) {
            plane.push_back(*iter);
        }
        else {
            top = plane.back();
            if (!Intersection(&i_p, top.gets(), top.gett(), iter->gets(), iter->gett())
                || abs(iter->getSlope()) - abs(top.getSlope()) < ERR) {
                // There is no intersection above.
                iter++;
                continue;
            }
            if ((i_p.gety() - top.gets().gety() < ERR)) {
                // prev intersection is not valid
                plane.pop_back();
                continue;
            }
            plane.pop_back();
            if (is_pos) {
                plane.push_back(Edge(top.gets(), i_p));
                plane.push_back(Edge(i_p, iter->gett()));
            }
            else {
                plane.push_back(Edge(i_p, top.gett()));
                plane.push_back(Edge(iter->gets(), i_p));
            }
        }
        iter++;
    }
    plane[0] = getExtension(plane[0], !is_pos);
    temp = getExtension(plane.back(), is_pos);
    plane.pop_back();
    plane.push_back(temp);
}

void interPlane(std::vector<Point>& inter_plane, std::vector<Edge>& pos_plane, std::vector<Edge>& neg_plane) {
    std::vector<Edge>::iterator pos = pos_plane.begin();
    std::vector<Edge>::iterator neg = neg_plane.begin();
    while (pos->crossing(*neg, true) == nullptr) {
        pos->gett().gety() > neg->gets().gety() ? neg++ : pos++;
        if (pos == pos_plane.end() || neg == neg_plane.end())
            return;
    }

    Point i_p;
    Intersection(&i_p, pos->gets(), pos->gett(), neg->gets(), neg->gett());
    reverse(neg_plane.begin(), neg_plane.end());
    
    neg = neg_plane.begin();
    while (neg != neg_plane.end()) {
        if (neg->gets().gety() > i_p.gety())
            inter_plane.push_back(neg->gets());
        neg++;
    }

    inter_plane.push_back(i_p);
    
    pos = pos_plane.begin();
    while (pos != pos_plane.end()) {
        if (pos->gett().gety() > i_p.gety())
            inter_plane.push_back(pos->gett());
        pos++;
    }
}

void toEdges(std::vector<Point>& src, std::vector<Edge>& dest) {
    for (std::vector<Point>::iterator iter = src.begin() + 1; iter != src.end(); iter++)
        dest.push_back(Edge(*(iter - 1), *iter));
}

double distPointToEdge(Point p, Edge e) {
    double dy = (e.gets().gety() - e.gett().gety());
    double dx = (e.gets().getx() - e.gett().getx());
    double a = dy / dx;
    double b = -a * e.gets().getx() + e.gets().gety();
    return abs(a * p.getx() + b - p.gety());
}

bool isMatchingEdge(Point i, Edge j) { 
    return (j.gets().getx() - i.getx() <= ERR) && (i.getx() - j.gett().getx() <= ERR);
}

void calcMinTower(double *tower_x, double *tower_len, std::vector<Edge>& edges, std::vector<Point>& vertices) {
    double temp;
    std::vector<Point>::iterator i = vertices.begin();
    std::vector<Edge>::iterator j = edges.begin();
    while (i != vertices.end()) {
        if (isMatchingEdge(*i, *j)) {
            temp = distPointToEdge(*i, *j);
            if (temp < *tower_len) {
                *tower_len = temp;
                *tower_x = i->getx();
            }
            i++;
        }
        else {
            j++;
        }
    }
}

int main() {
    int n;
    double x, y;
    Point prev;
    Point cur;
    std::vector<Point> vertices;
    std::vector<Edge> edges, pos_edge, neg_edge;
    std::cin >> n;
    for (int i = 0; i < n; i++) {
        std::cin >> x >> y;
        cur = Point(x, y);
        vertices.push_back(cur);
        if (i != 0) {
            edges.push_back(Edge(prev, cur));
            if (edges.back().getSlope() >= 0)
                pos_edge.push_back(edges.back());
            else
                neg_edge.insert(neg_edge.begin(), edges.back());
        }
        prev = cur;
    }

    std::vector<Edge> pos_plane;
    std::vector<Edge> neg_plane;
    calcHalfPlane(pos_plane, pos_edge);
    calcHalfPlane(neg_plane, neg_edge);

    std::vector<Point> inter_plane_p;
    std::vector<Edge> inter_plane_e;
    interPlane(inter_plane_p, pos_plane, neg_plane);
    
    if (inter_plane_p.empty()) {
        std::cout << "There is no feasible solution." << std::endl;
        return 0;
    }

    double tower_x, tower_len = MAX_X;
    toEdges(inter_plane_p, inter_plane_e); 
    inter_plane_p.pop_back();
    inter_plane_p.erase(inter_plane_p.begin());
    calcMinTower(&tower_x, &tower_len, edges, inter_plane_p);
    calcMinTower(&tower_x, &tower_len, inter_plane_e, vertices);

    std::cout << tower_x << std::endl << tower_len << std::endl;

    return 0;
}