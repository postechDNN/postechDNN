#include "Gen_geom_data.h"

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
std::vector<double> Gen_geom_data::randomAngleSteps(int n) {
    std::vector<double> angles; //angles�� ������ ���� ����
    std::default_random_engine generator; //default_random_engine ���̺귯���� �̿��ؼ� generator�� random�� pt�� ����
    std::uniform_real_distribution<double> distribution(-1, 1);//ǥ�� ���̺귯�� ���

    double lower = (2.0 * this->const_pi() / n) - 1;
    double upper = (2.0 * this->const_pi() / n) + 1;
    double cumsum = 0;

    for (int i = 0; i < n; ++i) {
        double angle = std::uniform_real_distribution<double>(lower, upper)(generator);
        angles.push_back(angle);
        cumsum += angle;
    }

    // Normalize the steps so that point 0 and point n+1 are the same
    cumsum /= (2.0 * this->const_pi());
    for (int i = 0; i < n; ++i) {
        angles[i] /= cumsum;
    }

    return angles;
}

// Function to clip a value within a specified range
double Gen_geom_data::clip(double value, double lower, double upper) {
    return std::min(upper, std::max(value, lower));
}

// Function to generate a polygon, center�� (0,0)���� ����, avgRadius�� 10���� ����
SimplePolygon Gen_geom_data::gen_simple_polygon(int n) {

    std::vector<double> angleSteps = this->randomAngleSteps(n);

    // Generate the points
    std::vector<Point> pts;
    std::vector<std::pair<double, double>> points;//points�� ������ ���� ����
    std::default_random_engine generator;
    std::normal_distribution<double> normalDistribution(30);
    std::uniform_real_distribution<double> uniformDistribution(0.0, 2 * this->const_pi());

    double angle = uniformDistribution(generator);

    for (int i = 0; i < n; ++i) {
        double radius = this->clip(uniformDistribution(generator), 0, 2 * 30);
        double x = radius * std::cos(angle);
        double y = radius * std::sin(angle);
        pts.push_back(Point(x,y));
        //points.push_back(std::make_pair(x, y));
        angle += angleSteps[i];
    }

    //return points;
    std::reverse(pts.begin(),pts.end());
    return SimplePolygon(pts);
}

Graph<Point> Gen_geom_data::gen_planar_graph(int n){
    std::vector<Point> vertices = this->gen_points_uniform(n);
    Graph<Point> graph;
    //std::vector<std::vector<int>> matrix;
    //matrix.resize(n,std::vector<int>(n,0));

    std::priority_queue<std::pair<int,int>> node_degrees;
    for(int i = 0;i<n;i++){
        node_degrees.push(std::pair<int,int>(0,i));
        graph.insert_vertex(vertices[i]);
    }

    std::vector<std::pair<int,int>> edges;
    std::uniform_int_distribution<int> dis(0,n-1);
    std::uniform_int_distribution<int> dis2(1,3*n-6);
    int m = dis2(this->gen);

    while (m>0){
        int deg,u;
        std::tie(deg, u) = node_degrees.top();
        int v= u; 
        while(u == v)
            v= dis(this->gen);
        Edge e(vertices[u],vertices[v]);

        bool intersect = false;
        for(int i = 0; i< edges.size();i++){
            int u_t,v_t;
            std::tie(u_t,v_t) = edges[i];
            Edge e_t(vertices[u_t],vertices[v_t]);
            if(e.crossing(e_t,false)){
                intersect = true;
                break;
            }
        }
        if(!intersect){
            //matrix[u][v] = matrix[v][u] = 1;
            edges.push_back(std::pair<int,int>(u,v)); 
            graph.insert_edge(u,v);
        }
        m--;
        node_degrees.pop();
        node_degrees.push(std::pair<int,int>(deg-1,u));
    }

    return graph;
} 

 std::vector<SimplePolygon> Gen_geom_data::gen_polygonal_domain(int n, int num_polys){
    ;
 }
Gen_geom_data::~Gen_geom_data(){}
