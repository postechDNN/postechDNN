#include "Point.h"
#include <cmath>
#include <iostream>
#include <fstream>

//this class defines a point in 2D space
#define ERR 1e-6
Point::Point() {
}
Point::Point(int _n) {
	this->n = _n;
	//std::vector<int> V = { 1,2,3 };
	//this->xs = V;
	this->xs = std::vector<double>(_n, 0.);
	//this->x = 0.;
	//this->y = 0.;
	//this->z = 0.;
}
Point::Point(int _n, bool isFreePoint) {
	this->n = _n;
	this->xs = std::vector<double>(_n, 0.);
	this->is_Free_Point = isFreePoint;
}
Point::Point(std::vector<double> _xs) {
	this->xs = _xs;
	this->n = _xs.size();
	//this->x = _x;
	//this->y = _y;
	//this->z = _z;
}
Point::Point(std::vector<double> _xs, bool isFreePoint) {
	this->xs = _xs;
	this->n = _xs.size();
	this->is_Free_Point = isFreePoint;
}
Point::Point(Point* _p) {
	this->xs = _p->getxs();
	this->n = this->xs.size();
}
Point::~Point() {}

bool Point::operator==(Point* _p) {
	for (int i=0;i<this->n;i++){
		if (std::abs(this->xs[i] - _p->getx(i)) > ERR) return false;
	}
	return true;
}

Point Point::operator- (Point* _p) {
	int _n = this->n;
	std::vector<double> v;
	v = std::vector<double>(_n, 0.);
	for (int i=0;i<this->n;i++){
		v[i] = this->xs[i] - _p->getx(i);
	}
	Point p(this->xs = v);
	return p;
}

double Point::getx(int _i) {
	return this->xs[_i];
}

void Point::setx(int _i, double _x) {
	this->xs[_i] = _x;
}

std::vector<double> Point::getxs() {
	return this->xs;
}

void Point::setxs(std::vector<double> _xs) {
	this->xs = _xs;
}

int Point::getsize() {
	return this->n;
}


double Point::distance(Point* _p) {
	double a = 0;
	for (int i=0;i<this->n;i++){
		a += pow(this->xs[i] - _p->getx(i), 2);
	}
	return sqrt(a);
}

void Point::print() {
	
	std::cout << "(";
	for (int j = 0; j < this->xs.size(); j++) {
		if (j == this->xs.size() - 1) {
			std::cout << this->getx(j) << ") ";
		}
		else {
			std::cout << this->getx(j) << " ";
		}
	}
}

void Point::print(std::string dir) {
	std::ofstream fout; 
	fout.open(dir);
	
	fout << "(";
	for (int j = 0; j < this->xs.size(); j++) {
		if (j == this->xs.size() - 1) {
			fout << this->getx(j) << ") ";
		}
		else {
			fout << this->getx(j) << " ";
		}
	}

	fout.close();
}

double distanceBtwFreePoints(Free_Point* p1, Free_Point* p2) {
	double a = 0;
	for (int i = 0; i < p1->xs.size(); i++) {
		a += pow(p1->xs[i] - p2->getx(i), 2);
	}
	return sqrt(a);
}

double distanceBtwGPandFP(Grid_Point* p1, Free_Point* p2) {
	double a = 0;
	for (int i = 0; i < p1->xs.size(); i++) {
		a += pow(p1->xs[i] - p2->getx(i), 2);
	}
	return sqrt(a);
}

int numTotalPoints = 0;

Free_Point::Free_Point(Point* _p) :Point(xs) {
	xs = _p->getxs();
	id = numTotalPoints; numTotalPoints++;
}

Free_Point::Free_Point(std::vector<double> _xs) :Point(xs) {
	xs = _xs;
	id = numTotalPoints; numTotalPoints++;
}

void Free_Point::set_maxmin() {
	for (int i = 0; i < this->n; i++) {
		this->xs_max.push_back(DBL_MIN);
		this->xs_min.push_back(DBL_MAX);
	}
	for (int i = 0; i < this->n; i++) {
		this->xs_max[i] = (this->xs_max[i] > this->getx(i)) ? this->xs_max[i] : this->getx(i);
		this->xs_min[i] = (this->xs_min[i] < this->getx(i)) ? this->xs_min[i] : this->getx(i);
	}
}

Grid_Point::Grid_Point(int n) : Point(n) { ind = std::vector<long long int>(n,-1); num = -1; 
	ip_u = std::vector<bool>(n,false); ip_d = std::vector<bool>(n, false); encl = -1; }

Grid_Point::Grid_Point(std::vector<long long int> _ind, Point* _upper_left, 
	double _eps, std::vector<long long int> _xs)
{
	n = _upper_left->n;
	ind = _ind;
	xs = std::vector<double>(n, 0);
	for (int i = 0; i < n;i++) {
		xs[i] = _upper_left->xs[i] + ind[i] * _eps;
	}
	num = 0;

	long long int mult = 1;
	for (int i = 0; i < n;i++) {
		//add num!!!!!!!!!
		num += mult * _ind[n - 1 - i];
		mult *= _xs[n - 1 - i];
	}

	encl = -1;
	ip_u = std::vector<bool>(n, false);
	ip_d = std::vector<bool>(n, false);
}

//Grid_Point::Grid_Point(int _x_num, int _y_num, int _z_num, double x_min, double y_min, double z_min, double eps, int eg_y, int eg_z) {
//	ind.x_ind = _x_num; ind.y_ind = _y_num; ind.z_ind = _z_num;
//	x = x_min + ind.x_ind * eps; y = y_min + ind.y_ind * eps; z = z_min + ind.z_ind * eps;
//	num = ind.x_ind * eg_y * eg_z + ind.y_ind * eg_z + ind.z_ind;
//	encl = -1;
//	ip = { false, false, false, false, false, false };
//}
