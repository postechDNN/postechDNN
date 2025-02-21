#include "Gen_geom_data.h"
#include <iostream>
#include <fstream>
#include <string>

Gen_geom_data::Gen_geom_data(Point left_bottom, Point right_top) {
    this->min_x = left_bottom.getx();
    this->max_x = right_top.getx();
    this->min_y = left_bottom.gety();
    this->max_y = right_top.gety();
    std::random_device rd;
    std::mt19937 gen(rd());
}

std::vector<Point> Gen_geom_data::gen_points_uniform(int n) {

    std::uniform_real_distribution<> u_dis_x(this->min_x, this->max_x);
    std::uniform_real_distribution<> u_dis_y(this->min_y, this->max_y);

    std::vector<Point> ret;

    for (int i = 0; i < n; i++) {
        Point point(u_dis_x(gen), u_dis_y(gen));
        ret.push_back(point);
    }

    return ret;
}

std::vector<Point> Gen_geom_data::gen_points_gaussian(int n, Point mean, double dev_x, double dev_y) {
    std::normal_distribution<> n_dis_x(mean.getx(), dev_x);
    std::normal_distribution<> n_dis_y(mean.gety(), dev_y);

    std::vector<Point> ret;

    for (int i = 0; i < n; i++) {
        double x = n_dis_x(gen);
        double y = n_dis_y(gen);

        double mapped_x = 1 / (1 + std::exp(-x));
        double mapped_y = 1 / (1 + std::exp(-y));
        mapped_x *= (max_x - min_x);
        mapped_y *= mapped_y * (max_y - min_y);
        Point point(mapped_x, mapped_y);
        ret.push_back(point);
    }

    return ret;
}


// Function to generate random angle steps
std::vector<double> Gen_geom_data::randomAngleSteps(int n, double irregularity) {
    std::vector<double> angles;
    std::random_device rd;
    std::mt19937 gen(rd());

    double lower = (2.0 * this->const_pi() / n) - irregularity;
    double upper = (2.0 * this->const_pi() / n) + irregularity;
    double cumsum = 0;

    std::uniform_real_distribution<double> dis(lower, upper);
    double angle;

    for (int i = 0; i < n; i++) {
        angle = dis(gen);
        angles.push_back(angle);
        cumsum += angle;
    }

    // Normalize the steps so that point 0 and point n+1 are the same
    cumsum /= (2.0 * this->const_pi());
    for (int i = 0; i < n; i++) {
        angles[i] /= cumsum;
    }

    return angles;
}

// Function to clip a value within a specified range
double Gen_geom_data::clip(double value, double lower_1, double upper_1) {
    //double lower_1 = 0;
    //double upper_1 = std::max((this->max_x - this->min_x) / 2, (this->max_y - this->min_y) / 2);
    return std::min(upper_1, std::max(value, lower_1));
}

// Function to generate a polygon
SimplePolygon Gen_geom_data::gen_simple_polygon(int n, double center_x, double center_y) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0, 1);

    double irr = dis(gen);
    irr *= 2 * const_pi() / n;

    std::vector<double> angleSteps = this->randomAngleSteps(n, irr);

    // Generate the points
    std::vector<Point> pts;
    std::uniform_real_distribution<double> uniformDistribution_angle(0.0, 2 * this->const_pi());
    std::uniform_real_distribution<double> uniformDistribution_radius(0.0, std::max((this->max_x - this->min_x) / 4, (this->max_y - this->min_y) / 4));

    double angle = uniformDistribution_angle(gen);
    double radius;
    double x, y;

    for (int i = 0; i < n; ++i) {
        radius = uniformDistribution_radius(gen);
            //clip(uniformDistribution(gen), 0, std::max((this->max_x - this->min_x) / 2, (this->max_y - this->min_y) / 2));
        x = center_x + radius * std::cos(angle);
        y = center_y + radius * std::sin(angle);
        pts.push_back(Point(x, y));
        angle += angleSteps[i];
    }

    std::reverse(pts.begin(), pts.end());
    return SimplePolygon(pts);
}

// function to generate obstacles as DCEL data
DCEL Gen_geom_data::gen_polygonal_domain(int n) {
    // input n: sum of vertices for all obstacle

    std::vector<SimplePolygon> ret;
    std::uniform_real_distribution<double> x_gen(min_x, max_x);
    std::uniform_real_distribution<double> y_gen(min_y, max_y);

    // STEP 1. decide the number of obstacles and decide the number of its vertices for each obstacle (>=3)
    // m: the number of obstacle, random (1 ~ n/3)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> m_gen(1, n / 3);
    int m = m_gen(gen);
    int ver_n = n;

    // std::cout << m << std::endl;
    
    // STEP 2. generate obstacles one by one in a bounding box using "gen_simple_polygon"
    for (int i = m; i > 0; i--) {
        // k: the number of its vertices
        std::uniform_int_distribution<int> k_gen(3, n - 3 * i + 3);
        int k;
        if (i == 1) {
            k = n;
        } else {
            k = k_gen(gen);
        }
        n -= k;

        double center_x = x_gen(gen);
        double center_y = y_gen(gen);

        bool flag;

        // generate obstacles as Simple Polygon data
        SimplePolygon tmp = gen_simple_polygon(k, center_x, center_y);
        
        // check the generated obstacle(tmp) intersects with others(sim)
        // flag: if two edges are crossing then true, otherwise false
        do {
            flag = false;
            center_x = x_gen(gen);
            center_y = y_gen(gen);
            tmp = gen_simple_polygon(k, center_x, center_y);

            for (SimplePolygon sim: ret) {
                for (auto a : sim.getEdges()) {
                    for (auto b : tmp.getEdges()) {
                        // if two edges are crossing, make simple polygon again
                        if (a.crossing(b, true) != nullptr) {
                            flag = true;
                            break;
                        }
                    }
                    if (flag) {
                        break;
                    }
                }
                if (flag) {
                    break;
                }
            }
        } while (flag);
        ret.push_back(tmp);
    }

    // STEP 3. convert generate obstacles (type of SimplePolygon) into a planar graph (type of Adjacent-List)
    // adjList: Adjacent-list for all Simple Polygon (express the connection between vertices)
    std::vector<std::vector<int>> adjList;
    // allVerList: Vertex list (Point data) for all Simple Polygon - to make DCEL data
    std::vector<Point> allVerList;

    for (auto x : ret) {
        // verList: Vertex list (Point data) for one Simple Polygon
        std::vector<Point> verList = x.getVertices();

        allVerList.insert(allVerList.end(), verList.begin(), verList.end());
        
        int size = verList.size();
        int exist = adjList.size();

        for (int i = 0; i < size; i++) {
            int prev = (i - 1 < 0) ? size - 1 : i - 1;
            int next = (i + 1 >= size) ? 0 : i + 1;

            std::vector<int> adjTmp = {prev+exist, next+exist};
            adjList.push_back(adjTmp);
        }
    }

    // std::string line;
    // std::ofstream file("output1.txt");
    // if (file.is_open()) {
    //     file << ver_n << "\n"; // the number of Point
    //     for (auto v : allVerList) {
    //         file << v.getx() << " " << v.gety() << "\n";
    //     } // Point x, y info
    //     file << 2*ver_n << "\n"; // the number of Edge
    //     for (int i = 0; i < ver_n; i++) {
    //         file << i << " " << adjList[i][0] << "\n";
    //         file << i << " " << adjList[i][1] << "\n";
    //     }
    //     file.close();
    // } else {
    //     std::cout << "error" << std::endl;
    // }

    // STEP 4. convert the planar graph into DCEL data. (use DCEL(std::vector<Point>&, std::vector<std::vector<int>>&,std::string key ="__default__") in DCEL_operation.cpp)
    DCEL dcel = DCEL(allVerList, adjList);

    return dcel;
}


std::vector<SimplePolygon> Gen_geom_data::gen_polygonal_domain(int n, int m) {
    std::vector<SimplePolygon> ret;
    std::uniform_real_distribution<double> x_gen(min_x, max_x);
    std::uniform_real_distribution<double> y_gen(min_y, max_y);

    for (int i = m; i > 0; i--) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> k_gen(3, n - 3 * i + 3);
        int k = k_gen(gen);
        double center_x = x_gen(gen);
        double center_y = y_gen(gen);

        n -= k;
        bool flag;
        SimplePolygon tmp = gen_simple_polygon(k, center_x, center_y);
        do {
            flag = false;
            tmp = gen_simple_polygon(k, center_x, center_y);
            for (SimplePolygon sim : ret) {
                for (auto a : sim.getEdges()) {
                    for (auto b : tmp.getEdges()) {
                        if (a.crossing(b, true) != nullptr) {
                            flag = true;
                            break;
                        }
                    }
                    if (flag) {
                        break;
                    }
                }
                std::vector<Point> tmpvs = tmp.getVertices();
                for (Point pt : tmpvs) {
                    if (sim.inPolygon(pt) != -1) {
                        flag = true;
                        break;
                    }
                } 
                if (flag) {
                    break;
                }
            }
        }
        while (flag);
        ret.push_back(tmp);
    }
    return ret;
}

std::vector<Point> Gen_geom_data::gen_sources(std::vector<SimplePolygon>& obstacles, int n) {
    std::uniform_real_distribution<> u_dis_x(this->min_x, this->max_x);
    std::uniform_real_distribution<> u_dis_y(this->min_y, this->max_y);

    std::vector<Point> ret;

    for (int i = 0; i < n+1; i++) {
        bool flag;
        Point point;
        do {
            flag = false;
            point.setx(u_dis_x(gen));
            point.sety(u_dis_y(gen));
            for (SimplePolygon sim : obstacles) {
                if (sim.inPolygon(point) != -1) {
                    flag = true;
                    break;
                }
            }
        } while (flag);  
        ret.push_back(point);
    }
    return ret;
}

Graph<Point> Gen_geom_data::gen_planar_graph(int n) {
    std::vector<Point> vertices = this->gen_points_uniform(n);
    Graph<Point> graph;
    //std::vector<std::vector<int>> matrix;
    //matrix.resize(n,std::vector<int>(n,0));

    std::priority_queue<std::pair<int, int>> node_degrees;
    for (int i = 0;i < n;i++) {
        node_degrees.push(std::pair<int, int>(0, i));
        graph.insert_vertex(vertices[i]);
    }

    std::vector<std::pair<int, int>> edges;
    std::uniform_int_distribution<int> dis(0, n - 1);
    std::uniform_int_distribution<int> dis2(1, 3 * n - 6);
    int m = dis2(this->gen);

    while (m > 0) {
        int deg, u;
        std::tie(deg, u) = node_degrees.top();
        int v = u;
        while (u == v)
            v = dis(this->gen);
        Edge e(vertices[u], vertices[v]);

        bool intersect = false;
        for (int i = 0; i < edges.size();i++) {
            int u_t, v_t;
            std::tie(u_t, v_t) = edges[i];
            Edge e_t(vertices[u_t], vertices[v_t]);
            if (e.crossing(e_t, false)) {
                intersect = true;
                break;
            }
        }
        if (!intersect) {
            //matrix[u][v] = matrix[v][u] = 1;
            edges.push_back(std::pair<int, int>(u, v));
            graph.insert_edge(u, v);
        }
        m--;
        node_degrees.pop();
        node_degrees.push(std::pair<int, int>(deg - 1, u));
    }

    return graph;
}


Gen_geom_data::~Gen_geom_data() {}

