#pragma once
#include "Space.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>



void text_input(std::vector<Point>& srcs, std::vector<Box>& boxes, Point& q, string fname) {
    std::ifstream file(fname);
    int n, m, d;
    file >> n >> m >> d;
    for (size_t i = 0; i < n; i++) {
        std::vector<double> temp(d,0.);
        for (size_t j = 0; j < d; j++) {
            file >> temp[j];
        }
        srcs.emplace_back(temp);
    }
    for (size_t i = 0; i < m; i++) {
        std::vector<std::vector<double>> temp(2,std::vector<double>(d,0.));
        for (size_t j = 0; j < 2; j++) {
            for (size_t k = 0; k < d; k++) {
                file >> temp[j][k];
            }
        }
        boxes.emplace_back(d,temp[0],temp[1]);
    }
    std::vector<double> temp(d, 0.);
    for (size_t i = 0; i < d; i++) {
        file >> temp[i];
    }
    q = Point(temp);
    file.close();
}

void print_result(BoxGraphL2& s, Point q, std::tuple<Point, int, double> answer, string fname) {
    std::ofstream file(fname);
    file << s.sites.size() << " " << s.Boxes.size() << " " << s.d << std::endl;
    for (size_t i = 0; i < s.sites.size(); i++) {
        for (size_t j = 0; j < s.d; j++) {
            file << s.sites[i].getx(j) << " ";
        }
        file << std::endl;
    }
    for (size_t i = 0; i < s.Boxes.size(); i++) {
        std::vector<std::vector<double>> temp = {s.Boxes[i].min, s.Boxes[i].max};
        for (size_t j = 0; j < 2; j++) {
            for (size_t k = 0; k < s.d; k++) {
                file << static_cast<int>(temp[j][k]) << " ";
            }
            file << std::endl;
        }
    }
    for (size_t i = 0; i < s.d; i++) {
        file << q.getx(i) << " ";
    }
    file << std::endl;
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
                if (abs(s.adj_list[temp_ind][i].second + s.dists[s.adj_list[temp_ind][i].first] - temp_dist) < 1e-8) {
                    temp_ind = s.adj_list[temp_ind][i].first;
                    p2 = s.vertices[temp_ind];
                    path.push_back(p2);
                }
            }
        }
    }
    file << path.size() << std::endl;
    for (size_t i = 0; i < path.size(); i++) {
        for (size_t j = 0; j < s.d; j++) {
            file << path[i].getx(j) << " ";
        }
        file << std::endl;
    }
    file.close();
}