#include "Box.h"
#include "Space.h"
/*

std::vector<std::pair<Point, Point>> d_axis(int d, Box vertex_1, Box vertex_2) {

    std::vector<std::pair<Point, Point>> result;

    for (int i = 0; i < d; ++i) {
        Point p1(vertex_1.vertices[i].xs);
        Point p2(vertex_2.vertices[i].xs);
        result.push_back(std::make_pair(p1, p2));
    }
    return result;
};

*/

std::vector<std::vector<double> > Box::generate_epsilon(double rmin, double epsilon) {
    //vector<Point> result�� std::vector<std::pair<Point, Point>> d_axis�� output��

    std::vector < std::vector<double>> result_2;

    //double dim = space_a->d;

    double result_1 = pow(this->d, 3.0 / 2.0);
    double s = rmin /( 2 * result_1);

    for (int i = 0; i < this->d; i++) {
        double step;
        step = floor((max[i] - min[i]) / s);
        double margin = ((max[i] - min[i]) - step * s) / 2;

        vector<double> temp;
        for (int j = 0; j < step+1; j++) {
            temp.push_back(min[i] + margin + (j * step));
        }
        result_2.push_back(temp);
    }

    return result_2;
};


/*
Box vertex_1;
//Point a = this->Box;
Box vertex_1. = this->Box.min;
//Box a.min;
double min = vertex_1.vertices[0].xs;
double max = result.second;
*/

//input: void d_axis();�� d���� ��
//output: �� ���� �����ŭ�� �������� �ڸ���
/*
    double step;
    step = floor((max - min) / s);
    double j = floor((max - min) / s); //epsilon�� ���̿��� �ԽǷ�, s=s'

    double margin = ((max - min) - step * s) / 2;
*/
Box::Box(int n,  vector<double> min, vector<double> max) {
    this->d = n;
    this->min = min;
    this->max = max;
}
Box::~Box(){
}

