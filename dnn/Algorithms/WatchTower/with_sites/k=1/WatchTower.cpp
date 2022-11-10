#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include "Site.h"
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

bool isValidTurn(const Point& a, const Point& b, const Point& c, bool isReverse) {
    if(isReverse)
        return Edge(a, c).getSlope() - Edge(a, b).getSlope() <= ERR;
    return Edge(a, b).getSlope() - Edge(a, c).getSlope() <= ERR;
}

void addPoint(std::vector<Point>& hull, const Point& p, bool isReverse) {
    Point p1, p2;
    while (1) {
        if (hull.size() < 2) {
            hull.push_back(p);
            break;
        }
        else {
            p1 = hull[hull.size() - 2];
            p2 = hull.back();
            if (isValidTurn(p1, p2, p, isReverse))
                hull.pop_back();
            else {
                hull.push_back(p);
                break;
            }
        }
    }    
}

void calValidSlope(std::vector<Point>& vertices, std::vector<Site>& sites) {
    std::vector<Point> hull;
    std::vector<Point>::iterator i = vertices.begin();
    std::vector<Site>::iterator j = sites.begin();
    Edge e, maxSlope;
    while (i->getx() < j->getPoint().getx())
        i++;

    while (i != vertices.end() && j != sites.end()) {
        if (i->getx() < j->getPoint().getx()) {
            addPoint(hull, *i, false);
            i++;
        }
        else {
            addPoint(hull, j->getPoint(), false);
            if (hull.size() < 2) {
                j++;
                continue;
            }
            e = Edge(hull[hull.size() - 2], hull.back());
            if (e.getSlope() > 0) {
                maxSlope = abs(e.getSlope()) < abs(j->getPosSlope().getSlope()) ? j->getPosSlope() : e;
                j->setPosSlope(maxSlope);
            }
            else {
                maxSlope = abs(e.getSlope()) < abs(j->getNegSlope().getSlope()) ? j->getNegSlope() : e;
                j->setNegSlope(maxSlope);
            }
            j++;
        }
    }
    
    hull.clear();
    std::vector<Point>::reverse_iterator l = vertices.rbegin();
    std::vector<Site>::reverse_iterator m = sites.rbegin();
    while (l->getx() > m->getPoint().getx())
        l++;

    while (l != vertices.rend() && m != sites.rend()) {
        if (l->getx() > m->getPoint().getx()) {
            addPoint(hull, *l, true);
            l++;
        }
        else {
            addPoint(hull, m->getPoint(), true);
            if (hull.size() < 2) {
                m++;
                continue;
            }
            e = Edge(hull.back(), hull[hull.size() - 2]);
            if (e.getSlope() > 0) {
                maxSlope = abs(e.getSlope()) < abs(m->getPosSlope().getSlope()) ? m->getPosSlope() : e;
                m->setPosSlope(maxSlope);
            }
            else {
                maxSlope = abs(e.getSlope()) < abs(m->getNegSlope().getSlope()) ? m->getNegSlope() : e;
                m->setNegSlope(maxSlope);
            }
            m++;
        }
    }
}

int main() {
    int n, m;
    double x, y;
    Point prev;
    Point cur;
    std::vector<Point> vertices;
    std::vector<Site> sites;
    std::vector<Edge> edges, pos_edge, neg_edge;
    std::cin >> n >> m;
    for (int i = 0; i < n; i++) {
        std::cin >> x >> y;
        cur = Point(x, y);
        vertices.push_back(cur);
        if (i != 0) {
            edges.push_back(Edge(prev, cur));
        }
        prev = cur;
    }

    std::vector<Edge>::iterator iter = edges.begin();
    for (int i = 0; i < m; i++) {
        std::cin >> x >> y;
        cur = Point(x, y);
        while (!(iter->on(cur))) {
            iter++;
            if (iter == edges.end()) {
                std::cout << "Sites are not on the Terrain\n";
                return -1;
            }
        }
        sites.push_back(Site(cur, *iter));
        if (iter->getSlope() > 0)
            sites.back().setPosSlope(*iter);
        else
            sites.back().setNegSlope(*iter);
    }

    calValidSlope(vertices, sites);
    Edge notValid = Edge(Point(0, 0), Point(0.1, 0));
    for (std::vector<Site>::iterator iter = sites.begin(); iter != sites.end(); iter++) {
        if (!(iter->getPosSlope() == notValid))
            pos_edge.push_back(iter->getPosSlope());
        if (!(iter->getNegSlope() == notValid))
            neg_edge.insert(neg_edge.begin(), iter->getNegSlope());
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