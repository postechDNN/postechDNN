#ifndef CYCLE_GRAPH
#define CYCLE_GRAPH

#include <iostream>
#include <vector>
#include <cassert>
#include <stack>

class CycleGraph {
    public:
     int vertex_num;
     std::vector<int> weight; // index starts from 1.
     std::vector<int> edge; // edge[i] means the length of v_i to v_((i + 1) % vertex_num).
     
    private:
     int i_star; // start index of relabeled vertex.
     int center_left = -1, center_right = -1;
     int perimeter;
     double center_loc, radius;
     std::vector<int> edgeSum; // edgeSum[i] = countertclockwise length of v_1 to v_((i + 1) % vertex_num).
     std::vector<int> active_vertex;
     std::vector<std::pair<int, std::pair<double, double>>> active_interval;
    public:
     CycleGraph(const std::vector<int> &w, const std::vector<int> &e) {
        assert(w.size() >= 3);
        assert(w.size() == e.size());
    
        vertex_num = w.size() - 1;
        weight = w;
        edge = e;
        edgeSum = e;
        for (int i = 2; i <= vertex_num; i++)
            edgeSum[i] += edgeSum[i - 1];
        perimeter = edgeSum.back();
        radius = INT_MAX;
     }

     void printCenter() {
        getCenter();
        std::cout << "Center is between v" << center_left << " and v" << center_right << ", " << center_loc << " away from v" << center_left << ".\n";
     }
    private:
     double length(double loc1, double loc2) { // length between loc1 and loc2 (loc: counterclockwise location measured from v1.) 
        if (loc1 > loc2) std::swap(loc1, loc2);
        return std::min(loc2 - loc1, perimeter - loc2 + loc1);
     }
     double lengthV(int v_i, int v_j) { // length between vertex v_i and v_j.
        return length(edgeSum[v_i - 1], edgeSum[v_j - 1]);
     }
     bool isDominated(int v1, int v2, int v3) {
        std::pair<double, double> I1 = dominatingInterval(v2, v1);
        std::pair<double, double> I2 = dominatingInterval(v2, v3);
        std::pair<double, double> intersection = IntervalIntersection(I1, I2);
        if (intersection.first < 0) return true;
        else return false;
     }
     void relabelVertices() {
        int max_len = 0;
        for (int i = 2; i <= vertex_num; i++) {
            int length_to_vi = weight[i] * lengthV(1, i); 
            if (max_len < length_to_vi) {
                i_star = i;
                max_len = length_to_vi;
            }
        }
     }
     void getActiveVertices() {
        std::stack<int> L_a;
        L_a.push(i_star);
        if (i_star == vertex_num) L_a.push(1);
        else L_a.push(i_star + 1);

        for (int idx = 3; idx <= vertex_num; idx++) {
            int i = i_star + idx - 1;
            if (i > vertex_num) i %= vertex_num;
            if (isDominated(L_a.top(), i, i_star))
                continue;
            while (L_a.size() >= 3) {
                int first, second;
                first = L_a.top();
                L_a.pop();
                second = L_a.top();
                if (isDominated(second, first, i) == false) {
                    L_a.push(first);
                    break;
                }
            }
            L_a.push(i);
        }
        auto end = &L_a.top() + 1;
        auto begin = end - L_a.size();
        active_vertex = std::vector<int>(begin, end);
     }
     std::pair<double, double> dominatingInterval(int v1, int v2) {
        double loc1 = edgeSum[v1 - 1], loc2 = edgeSum[v2 - 1];
        double w1 = weight[v1], w2 = weight[v2];
        double mid_between, mid_antipodal;
        if (v1 < v2) {
            double len1 = abs(loc2 - loc1), len2 = perimeter - len1;
            mid_between = loc1 + (w2 / (w1 + w2)) * len1;
            mid_antipodal = loc2 + (w1 / (w1 + w2)) * len2;
            if (mid_between > mid_antipodal) mid_antipodal += perimeter;
            return std::make_pair(mid_between, mid_antipodal);
        } else {
            double len1 = abs(loc1 - loc2), len2 = perimeter - len1;
            mid_between = loc2 + (w1 / (w1 + w2)) * len1;
            mid_antipodal = loc1 + (w2 / (w1 + w2)) * len2;
            if (mid_antipodal > mid_between) mid_between += perimeter;
            return std::make_pair(mid_antipodal, mid_between);
        }
     }
     std::pair<double, double> IntervalIntersection
     (std::pair<double, double> I1, std::pair<double, double> I2) {
        if (I1.first > I2.first) std::swap(I1, I2);
        double l1 = I1.first, r1 = I1.second;
        double l2 = I2.first, r2 = I2.second;
        if (r1 > perimeter && r2 > perimeter) 
            return std::make_pair(std::max(l1, l2), std::min(r1, r2) - perimeter);  
        else if (r1 > perimeter) {
            if (l1 < r2) return std::make_pair(l1, r2);
            else if (l2 < r1 - perimeter) return std::make_pair(l2, r1 - perimeter);
        } else if (r2 > perimeter) {
            if (l2 < r1) return std::make_pair(l2, r1);
            else if (l1 < r2 - perimeter) return std::make_pair(l1, r2 - perimeter);
        } else {
            if (l2 < r1) return std::make_pair(l2, r1);
        }
        return std::make_pair(-1.0, -1.0);
     }
     void getDominatingIntervals() {
        int sz = (int)active_vertex.size();
        for (int idx = 0; idx < sz; idx++) {
            int i = active_vertex[idx];
            int l = (idx == 0) ? active_vertex.back() : active_vertex[idx - 1];
            int r = (idx == sz - 1) ? active_vertex[0] : active_vertex[idx + 1];
            std::pair<double, double> I1 = dominatingInterval(i, l);
            std::pair<double, double> I2 = dominatingInterval(i, r);
            std::pair<double, double> intersection = IntervalIntersection(I1, I2);
            active_interval.push_back({i, intersection});
        }
     }
     void getCenter() {
        relabelVertices();
        getActiveVertices();
        getDominatingIntervals();
        for (auto it: active_interval) {
            int vertex = it.first;
            double I_l = it.second.first, I_r = it.second.second;
            double l1 = weight[vertex] * length(edgeSum[vertex - 1], I_l);
            double l2 = weight[vertex] * length(edgeSum[vertex - 1], I_r);
            double loc = (l1 < l2) ? I_l : I_r;
            double l = std::min(l1, l2);
            if (l < radius) {
                center_loc = loc;
                radius = l; 
            }
        }
        for (int i = 1; i < vertex_num; i++) {
            double loc1 = edgeSum[i - 1], loc2 = edgeSum[i];
            if (loc1 <= center_loc && center_loc <= loc2) {
                center_left = i;
                center_right = i + 1;
                center_loc -= loc1;
                break;
            }
        }
        if (center_left == -1) {
            center_left = vertex_num;
            center_right = 1;
            center_loc -= edgeSum[vertex_num - 1];
        }
     }   
};

#endif