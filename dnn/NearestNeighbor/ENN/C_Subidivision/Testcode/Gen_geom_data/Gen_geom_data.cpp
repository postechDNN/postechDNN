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


SimplePolygon Gen_geom_data::gen_simple_polygon(int n) {
    std::vector<Point> pts = { Point(1,1),Point(2,2),Point(3,3) };
    SimplePolygon a(pts);
    return a;
}


// Function to generate random angle steps
std::vector<double> randomAngleSteps(int n) {
    std::vector<double> angles; //angles의 데이터 구조 정의
    std::default_random_engine generator; //default_random_engine 라이브러리를 이용해서 generator가 random한 pt를 생성
    std::uniform_real_distribution<double> distribution(-1, 1);//표준 라이브러리 사용

    double lower = (2.0 * M_PI / n) - 1;
    double upper = (2.0 * M_PI / n) + 1;
    double cumsum = 0;

    for (int i = 0; i < n; ++i) {
        double angle = std::uniform_real_distribution<double>(lower, upper)(generator);
        angles.push_back(angle);
        cumsum += angle;
    }

    // Normalize the steps so that point 0 and point n+1 are the same
    cumsum /= (2.0 * M_PI);
    for (int i = 0; i < n; ++i) {
        angles[i] /= cumsum;
    }

    return angles;
}


// Function to clip a value within a specified range
double clip(double value, double lower, double upper) {
    return std::min(upper, std::max(value, lower));
}


// Function to generate a polygon, center를 (0,0)으로 지정, avgRadius도 10으로 생각
std::vector<std::pair<double, double>> gen_simple_polygon(int n) {

    std::vector<double> angleSteps = randomAngleSteps(n);

    // Generate the points
    std::vector<std::pair<double, double>> points;//points의 데이터 구조 정의
    std::default_random_engine generator;
    std::normal_distribution<double> normalDistribution(30);
    std::uniform_real_distribution<double> uniformDistribution(0.0, 2 * M_PI);

    double angle = uniformDistribution(generator);

    for (int i = 0; i < n; ++i) {
        double radius = clip(uniformDistribution(generator), 0, 2 * 30);
        double x = radius * std::cos(angle);
        double y = radius * std::sin(angle);
        points.push_back(std::make_pair(x, y));
        angle += angleSteps[i];
    }

    return points;
}

SimplePolygon Gen_geom_data::gen_simple_polygon(int n){
    return 0;
}

Gen_geom_data::~Gen_geom_data(){}
