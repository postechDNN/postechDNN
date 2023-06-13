#include "Polytope.h"
#include <queue>
#include <assert.h>
#include <random>
#include<Eigen/Dense>

#define BUFFERSIZE 1000
#define ERR 1e-6

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;

simplices::simplices() {
}
simplices::simplices(int n, vector<Point*> vec) {
	d = n;
	vertices = vec;
	A.resize(d, d + 1);
	for (int i = 0; i < d + 1; i++) {
		vector<double> tmp = vec[i]->getxs();
		for (int j = 0; j < d; j++) {
			A(j, i) = tmp[j];
		}
	}
}

bool simplices::intersect(simplices x) {

}

bool simplices::isIn(Point* p) {
	VectorXd b(d);
	vector<double> tmp = p->getxs();
	for (int j = 0; j < d; j++) {
		b(j) = tmp[j];
	}
	VectorXd x(d + 1);
	x = A.colPivHouseholderQr().solve(b);
	if (abs(x.sum() - 1) > ERR) {
		return false;
	}
	else {
		for (int j = 0; j < d + 1; j++) {
			if (x(j) < 0) {
				return false;
			}
		}
		return true;
	}
}

Polytope::Polytope() {};

Polytope::Polytope(FILE* input) {};

bool Polytope::isIn(Point* p) {
	for (auto simp1 : this->component) {
		bool in = simp1.isIn(p);
		if (in) {	return true;}
	}
	return false;
};

bool Polytope::intersect(Polytope P) {
	for (auto simp1 : this->component) {
		for (auto simp2 : P.component) {
			bool intersect = simp1.intersect(simp2);
			if (intersect) { return true; }
		}
	}
	return false;
};

bool Polytope::operator==(Polytope P) {
	if (this->ord == P.ord) {
		return true;
	}
	else return false;
};