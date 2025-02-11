#pragma once
#include "../ENN/C_Subidivision/DCEL/Polygon.h"
#include "Space.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>



void text_input(std::vector<Point>& srcs, std::vector<SimplePolygon>& obstacles, Point& q, string fname) {
    std::ifstream file(fname);
    int n, m;
    file >> n >> m;
    for (size_t i = 0; i < n; i++) {
        double x, y;
        file >> x >> y;
        srcs.emplace_back(x, y);
    }
    for (size_t i = 0; i < m; i++) {
        int k;
        file >> k;
        std::vector<Point> temp;
        for (size_t j = 0; j < k; j++) {
            double x, y;
            file >> x >> y;
            temp.emplace_back(x, y);
        }
        obstacles.emplace_back(temp);
    }
    double x, y;
    file >> x >> y;
    q.setx(x);
    q.sety(y);
    file.close();
}

void print_result(VisGraph& s, Point q, tuple<Point, int, double> answer, string fname) {
    std::ofstream file(fname);
    file << s.srcs.size() << " " << s.obstacles.size() << endl;
    for (size_t i = 0; i < s.srcs.size(); i++) {
        file << s.srcs[i].getx() << " " << s.srcs[i].gety() << endl;
    }
    for (size_t i = 0; i < s.obstacles.size(); i++) {
        std::vector<Point> pts = s.obstacles[i].getVertices();
        file << pts.size() << endl;
        for (size_t j = 0; j < pts.size(); j++) {
            file << pts[j].getx() << " " << pts[j].gety() << endl;
        }
    }
    file << q.getx() << " " << q.gety() << endl;
    //TODO: add path to the end of the file
    std::vector<Point> path;
    int temp_ind = get<1>(answer);
    double temp_dist = get<2>(answer);
    Point p1 = q;
    Point p2 = s.vertices[temp_ind];
    path.push_back(p1);
    path.push_back(p2);
    while (true) {
        pair<Point, Point> temp = { p1,p2 };
        p1 = p2;
        if (p2 == get<0>(answer)) break;
        else {
            temp_dist = s.dists[temp_ind];
            for (int i = 0; i < s.adj_list[temp_ind].size(); i++) {
                if (abs(s.adj_list[temp_ind][i].second + s.dists[s.adj_list[temp_ind][i].first] - temp_dist) < ERR) {
                    temp_ind = s.adj_list[temp_ind][i].first;
                    p2 = s.vertices[temp_ind];
                    path.push_back(p2);
                }
            }
        }
    }
    file << path.size() << endl;
    for (size_t i = 0; i < path.size(); i++) {
        file << path[i].getx() << " " << path[i].gety() << endl;
    }
    file.close();
}