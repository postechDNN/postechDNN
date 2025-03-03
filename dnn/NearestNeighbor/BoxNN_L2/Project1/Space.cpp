
#include "Space.h"
#include "Box.h"
#include <limits>
#include <queue>
#include <tuple>

template<typename T>
std::vector<std::vector<T>> Cartesian_Product(const std::vector<std::vector<T>>& items);


//Constructer
BoxGraphL2::BoxGraphL2(int _d, std::vector<Point> &_sites, std::vector<Box> &_Boxes, double _epsilon) {
    this->epsilon = _epsilon;
    set_Space(_d, _sites, _Boxes);
}

//Destructor
BoxGraphL2::~BoxGraphL2(){
}


void  BoxGraphL2::set_Space(int _d, std::vector<Point> &_sites, std::vector<Box> &_Boxes) {
    this->d = _d;
    this->sites = _sites;
    this->vertices = _sites;
    this->Boxes = _Boxes;
    cal_rmin();
    for (auto& b : Boxes) {
        std::vector<std::vector<double>> gen;
        for (size_t i = 0; i < d; i++) {
            gen.push_back({b.min[i],b.max[i]});
        }
        auto verts = Cartesian_Product(gen);
        for (auto& v : verts) {
            vertices.push_back(Point(v));
        }
    }
    auto steiners = gen_SteinerPoint();
    vertices.insert(vertices.end(), steiners.begin(), steiners.end());
    visibility_graph();
    Dijkstra();
}

/*
Generate the cartesian product of the given vectors
Input: d vectors with the same length n
Output: Vector of d-dimentional n^d points
*/
template<typename T>
std::vector<std::vector<T>> Cartesian_Product(const std::vector<std::vector<T>>& items) {
    
    int length = items.size();
    std::vector<int> indexes(length, 0);

    std::vector<std::vector<double>> result;

    while (true) {
        std::vector<double> arr(length);
        for (int i = 0; i < length; i++) {
            arr[i] = items[i][indexes[i]];
        }
        result.push_back(arr);

        int row = length - 1;
        indexes[row]++;

        while (indexes[row] == items[row].size()) {
            if (row == 0)
                return result;
            indexes[row] = 0;
            row--;
            indexes[row]++;
        }
    }
}


//Return the list of all 2-combination from 1 to d
std::vector<std::pair<int,int>> BoxGraphL2:: Combination() {

    std::vector<vector<int>> combinations;

    std::vector<bool> v(this->d);
    std::fill(v.end() - 2, v.end(), true);

    do {
        vector<int> temp;
        for (auto i = 0; i < d; ++i) {
            if (v[i]) {
                temp.push_back(i);
            }
        }
        combinations.push_back(temp);
    } while (std::next_permutation(v.begin(), v.end()));
    std::vector<std::pair<int,int>> comb;
    for (auto &c : combinations) {
        comb.emplace_back(c[0], c[1]);
    }
    return comb;

};


//Generate all Steiner points of the visibility graph
std::vector<Point> BoxGraphL2::gen_SteinerPoint() {

    std::vector<std::pair<int, int>> combination = this->Combination();
    std::vector<std::vector<double>> answer_1;
   
    std::vector<Point> answer_3;  //return vector

    for (auto box : Boxes) {
        std::vector<std::vector<double> > Range = box.generate_epsilon(this->rmin, this->epsilon);
        for (auto space : combination) {
            std::vector<std::vector<double>> fixed_Range = Range;

            int a = space.first;
            int b = space.second;

            std::vector<std::vector<double>> new_Range;
            for (int i = 0; i < fixed_Range.size(); ++i) {
                if (i != a && i != b) {
                    new_Range.push_back(fixed_Range[i]);
                }
            }
            std::vector<std::vector<double>> answer = Cartesian_Product(new_Range);

            double min_a = box.min[a];
            double min_b = box.min[b];
            double max_a = box.max[a];
            double max_b = box.max[b];

            std::vector<std::vector<double>> answer_2;
            answer_2 = { {min_a,min_b},{min_a,max_b},{max_a,min_b},{max_a,max_b} };

            for (auto& k : answer_2) {
                std::vector<std::vector<double>> answer_1 = answer; 
                for (int i = 0; i < answer_1.size(); i++) {
                    answer_1[i].insert(answer_1[i].begin()+a, k[0]);
                    answer_1[i].insert(answer_1[i].begin()+b, k[1]);
                    answer_3.push_back(Point(answer_1[i]));
                }
            }
        }
    }
    return answer_3;
};


//Calculate r_min from the configuration of sites and boxes
void BoxGraphL2::cal_rmin() {
    std::vector<double> results;
    double sum = 0.0;
    double diff;
    // Box vs Box
    for (int i = 0; i < Boxes.size(); i ++) {
        for (int j = i + 1; j < Boxes.size(); j ++) {
            sum = 0.0;

            for (int k = 0; k < d; k ++) {
                double a_ik = Boxes[i].min[k];
                double b_ik = Boxes[i].max[k];

                double a_jk = Boxes[j].min[k];
                double b_jk = Boxes[j].max[k];

                if (std::max(a_ik, a_jk) <= std::min(b_ik, b_jk)) {
                    diff = 0.0;
                } else {
                    if (b_ik < a_jk) {
                        diff = b_ik - a_jk;
                    } else if (b_jk < a_ik) {
                        diff = b_jk - a_ik;
                    }
                }

                sum += diff * diff;
            }

            results.push_back(sqrt(sum));
        }
    }
    // Point vs Point
    for (int i = 0; i < vertices.size(); i ++) {
        for (int j = i + 1; j < vertices.size(); j ++) {
            sum = 0.0;

            for (int k = 0; k < d; k ++) {
                double x_k = vertices[i].xs[k];
                double y_k = vertices[j].xs[k];

                diff = x_k - y_k;

                sum += diff * diff;
            }

            results.push_back(sqrt(sum));
        }
    }
    // Box vs Point
    for (int i = 0; i < Boxes.size(); i ++) {
        for (int j = 0; j < vertices.size(); j ++) {
            sum = 0.0;

            for (int k = 0; k < d; k ++) {
                double a_ik = Boxes[i].min[k];
                double b_ik = Boxes[i].max[k];

                double a_jk = vertices[j].xs[k];
                double b_jk = vertices[j].xs[k];

                if (std::max(a_ik, a_jk) <= std::min(b_ik, b_jk)) {
                    diff = 0.0;
                } else {
                    if (b_ik < a_jk) {
                        diff = b_ik - a_jk;
                    } else if (b_jk < a_ik) {
                        diff = b_jk - a_ik;
                    }
                }

                sum += diff * diff;
            }

            results.push_back(sqrt(sum));
        }
    }
    double rmin = *std::min_element(results.begin(), results.end());
    this->rmin = rmin;
};


//Build the visibility graph with the sites, boxes and Steiner points
void BoxGraphL2::visibility_graph() {
    this->adj_list.clear();
    for (long long int i = 0; i < size(this->vertices); i++) {
        vector<pair<long long int, double>> temp;
        for (long long int j = 0; j < size(this->vertices); j++) {
            bool intersect = false;
            for (auto box : this->Boxes) {
                if (box.intersect(vertices[i], vertices[j])) { intersect = true; break; }
            }
            if (intersect || i == j) { continue; }
            temp.push_back(make_pair(j,vertices[i].dist(vertices[j])));
        }
        this->adj_list.push_back(temp);
    }   
};


//Add a box and modify the visibility graph
void BoxGraphL2::add_Box(Box b) {
    this->Boxes.push_back(b);
    this->set_Space(d, sites, Boxes);
}


//Delete a box and modify the visibility graph
void BoxGraphL2::del_Box(int i) {
    this->Boxes.erase(Boxes.begin() + i);
    this->set_Space(d, sites, Boxes);
}


//Add a site and modify the visibility graph
void BoxGraphL2::add_vert(Point p) {
    this->sites.push_back(p);
    this->set_Space(d, sites, Boxes);
}


//Delete a site and modify the visibility graph
void BoxGraphL2::del_vert(int i) {
    this->sites.erase(sites.begin() + i);
    this->set_Space(d, sites, Boxes);
}


//Build the nearest neighbor data structure using Dijkstra algorithm
void BoxGraphL2::Dijkstra() {
    dists.assign(vertices.size(), std::numeric_limits<double>::max());
    visited.assign(vertices.size(), false);
    near_src.assign(vertices.size(), -1);
    std::priority_queue<std::tuple<double, int, int>, std::vector<std::tuple<double, int, int>>, greater<std::tuple<double, int, int>>> que = {};
    for (long long j = 0; j < sites.size(); j++) {
        que.emplace(0, j, j);
        dists[j] = 0;
        near_src[j] = j;
    }
    while (!que.empty()) {
        std::tuple<double, int, int> temp = que.top();
        que.pop();
        if (visited[get<1>(temp)] == true) {
            continue;
        }
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


bool comp(std::pair<double, Point> a, std::pair<double, Point> b) {
    return a.first < b.first;
}


//Find the nearest site from the query point and return the coordinate, index, and distance of the site
tuple<Point, int, double> BoxGraphL2::query(Point query) {
    Point Near;
    double dist = INFINITY;
    int index = -1;
    for (int i = 0; i < vertices.size(); i ++){
        bool check_intersect = false;

        for (int j = 0; j < Boxes.size(); j ++){
            check_intersect = Boxes[j].intersect(query, vertices[i]);
            if (check_intersect == true){
                break;
            }
        }

        if (check_intersect == false){
            double sum = 0.0;
            double diff = 0.0;
            double dist_temp = 0.0;

            double dist_prev = dists[i];
            Point near_temp = vertices[i];

            for (int k = 0; k < d; k ++) {
                double q_k = query.xs[k];
                double v_k = vertices[i].xs[k];

                diff = q_k - v_k;

                sum += diff * diff;
            }

            dist_temp = dist_prev + sqrt(sum);

            if (dist >= dist_temp){
                dist = dist_temp;
                Near = vertices[near_src[i]];
                index = i;
            }
        }
    }

    return { Near, index, dist };
}
