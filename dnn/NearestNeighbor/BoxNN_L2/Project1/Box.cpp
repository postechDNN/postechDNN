#include "Box.h"
#include <algorithm>

Box::Box(int n,  vector<double> min, vector<double> max) {
    this->d = n;
    this->min = min;
    this->max = max;

}

Box::~Box() {
}


//Produce the vectors of the d-th coordinates of steiner points for all axes 
std::vector<std::vector<double> > Box::generate_epsilon(double rmin, double epsilon) {
    //vector<Point> resultÀº std::vector<std::pair<Point, Point>> d_axisÀÇ output°ª

    std::vector < std::vector<double>> result_2(this->d);

    double result_1 = pow(this->d, 3.0 / 2.0);
    double s = rmin*epsilon / (2 * result_1);

    for (auto& v1:result_2) {
        int i = 0;
        double step;
        step = floor((max[i] - min[i]) / s);
        double margin = ((max[i] - min[i]) - step * s) / 2;

        for (int j = 0; j < step+1;j++) {
            v1.push_back( min[i] + margin + (j * s));
        }
        i++;
    };

    return result_2;


};

bool Box::intersect(Point start, Point end) {
    double lambda = 0.0;
    // Checking intersection in this->min[i]
    for (int i = 0; i < this->d; i++) {
        if (start.getx(i) == this->min[i] && this->min[i] == end.getx(i)) { continue; }
        lambda = (this->min[i] - end.getx(i)) / (start.getx(i) - end.getx(i));
        if (lambda == 0) {
            // Vertex case 1
            bool vertex1 = true;
            double lambda_plus = lambda + 0.0001;
            for (int j = 0; j < this->d; j++) {
                double temp = lambda_plus * start.getx(j) + (1 - lambda_plus) * end.getx(j);
                if (this->min[j] >= temp || this->max[j] <= temp) {
                    vertex1 = false;
                    break;
                }
            }
            if (vertex1) { return true; }
        }
        else if (lambda == 1) {
            // Vertex case 2
            double lambda_minus = lambda - 0.0001;
            bool vertex1 = true;
            for (int j = 0; j < this->d; j++) {
                double temp = lambda_minus * start.getx(j) + (1 - lambda_minus) * end.getx(j);
                if (this->min[j] >= temp || this->max[j] <= temp) {
                    vertex1 = false;
                    break;
                }
            }
            if (vertex1) { return true; }
        }
        else if (0 < lambda && lambda < 1) {
            // Edge case 1
            double lambda_minus = lambda - 0.0001;
            bool edge1 = true;
            for (int j = 0; j < this->d; j++) {
                double temp = lambda_minus * start.getx(j) + (1 - lambda_minus) * end.getx(j);
                if (this->min[j] >= temp || this->max[j] <= temp) {
                    edge1 = false;
                    break;
                }
            }
            if (edge1) { return true; }

            // Edge case 2
            edge1 = true;
            double lambda_plus = lambda + 0.0001;
            for (int j = 0; j < this->d; j++) {
                double temp = lambda_plus * start.getx(j) + (1 - lambda_plus) * end.getx(j);
                if (this->min[j] >= temp || this->max[j] <= temp) {
                    edge1 = false;
                    break;
                }
            }
            if (edge1) { return true; }

            // general case
            bool check = true;
            for (int j = 0; j < this->d; j++) {
                if (i == j) { continue; }
                double temp = lambda * start.getx(j) + (1 - lambda) * end.getx(j);
                if (this->min[j] >= temp || this->max[j] <= temp) {
                    check = false;
                    break;
                }
            }
            if (check) { return true; }
        }
    }

    // Checking intersection in this->max[i]
    for (int i = 0; i < this->d; i++) {
        if (start.getx(i) == this->max[i] && this->max[i] == end.getx(i)) { continue; }
        lambda = (this->max[i] - end.getx(i)) / (start.getx(i) - end.getx(i));
        if (lambda == 0) {
            // Vertex case 1
            bool vertex1 = true;
            double lambda_plus = lambda + 0.0001;
            for (int j = 0; j < this->d; j++) {
                double temp = lambda_plus * start.getx(j) + (1 - lambda_plus) * end.getx(j);
                if (this->min[j] >= temp || this->max[j] <= temp) {
                    vertex1 = false;
                    break;
                }
            }
            if (vertex1) { return true; }
        }
        else if (lambda == 1) {
            // Vertex case 2
            double lambda_minus = lambda - 0.0001;
            bool vertex1 = true;
            for (int j = 0; j < this->d; j++) {
                double temp = lambda_minus * start.getx(j) + (1 - lambda_minus) * end.getx(j);
                if (this->min[j] >= temp || this->max[j] <= temp) {
                    vertex1 = false;
                    break;
                }
            }
            if (vertex1) { return true; }
        }
        else if (0 < lambda && lambda < 1) {
            // Edge case 1
            double lambda_minus = lambda - 0.0001;
            bool edge1 = true;
            for (int j = 0; j < this->d; j++) {
                double temp = lambda_minus * start.getx(j) + (1 - lambda_minus) * end.getx(j);
                if (this->min[j] >= temp || this->max[j] <= temp) {
                    edge1 = false;
                    break;
                }
            }
            if (edge1) { return true; }

            // Edge case 2
            edge1 = true;
            double lambda_plus = lambda + 0.0001;
            for (int j = 0; j < this->d; j++) {
                double temp = lambda_plus * start.getx(j) + (1 - lambda_plus) * end.getx(j);
                if (this->min[j] >= temp || this->max[j] <= temp) {
                    edge1 = false;
                    break;
                }
            }
            if (edge1) { return true; }

            // general case
            bool check = true;
            for (int j = 0; j < this->d; j++) {
                if (i == j) { continue; }
                double temp = lambda * start.getx(j) + (1 - lambda) * end.getx(j);
                if (this->min[j] >= temp || this->max[j] <= temp) {
                    check = false;
                    break;
                }
            }
            if (check) { return true; }
        }
    }

    return false;

}

