#include "Polytope.h"
#include "optimization.h"
#include <queue>
#include <assert.h>
#include <random>
#include<Eigen/Dense>

#define BUFFERSIZE 1000
#define ERR 1e-6

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using namespace alglib;

simplices::simplices() {
}
simplices::~simplices() {};
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

MatrixXd simplices::getmatrix() {
	return A;
};

bool simplices::intersect(simplices x) {
	MatrixXd B = x.getmatrix();
	MatrixXd T(d + 1, 2*d + 2);
	T.block(0, 0, d, d + 1) = A;
	T.block(0, d+1, d, d + 1) = B;
	for (int i = 0; i < 2*d + 2; i++) {
		if (i < d + 1) T(d, i) = 1;
		else T(d, i) = -1;
	}
	Eigen::FullPivLU<Eigen::MatrixXd> Temp(T);
	MatrixXd N = Temp.kernel().cast<double>();
	int Row = N.rows();
	int Col = N.cols();

	real_2d_array A;
	double* p = new double[Col * (Row + 1)];
	for (int i = 0; i < Col * Row; i++) {
		p[i] = N(i / Col, i % Col);
	}
	for (int i = Col * Row; i < Col * (Row +1); i++) {
		p[i] = ERR;
	}
	A.setcontent(Row + 1, Col, p);

	double* low = new double[Row + 1] {0,};
	low[Row] = ERR;
	real_1d_array al;
	al.setcontent(Row + 1, low);

	double* upp = new double[Row + 1] {INFINITY, };
	real_1d_array au;
	au.setcontent(Row + 1, low);

	double* c1 = new double[Col] {1, };
	real_1d_array c;
	real_1d_array s;
	c.setcontent(Col, c1);
	s.setcontent(Col, c1);

	double* v_low = new double[Col] {-INFINITY, };
	real_1d_array bndl;
	bndl.setcontent(Col, v_low);

	double* v_upp = new double[Col] {INFINITY, };
	real_1d_array bndu;
	bndu.setcontent(Col, v_upp);

	// Setting
	real_1d_array sol;
	minlpstate state;
	minlpreport rep;
	minlpcreate(Row + 1, state);
	minlpsetcost(state, c);
	minlpsetbc(state, bndl, bndu);
	minlpsetlc2dense(state, A, al, au, Row+1);
	minlpsetscale(state, s);

	// Solve
	minlpoptimize(state);
	minlpresults(state, sol, rep);

	for (int i = 0; i < Col; i++) {
		if (sol[i] != 0.0) {
			return true;
		}
	}	
	return false;
}

bool simplices::isIn(Point* p) {
	MatrixXd T(d + 1, d + 1);
	VectorXd b(d + 1);
	vector<double> tmp = p->getxs();
	for (int j = 0; j < d; j++) {
		b(j) = tmp[j];
	}
	T.block(0, 0, d, d + 1) = A;
	for (int i = 0; i < d + 1; i++) {
		T(d, i) = 1;
	}
	b(d) = 1;
	VectorXd x(d + 1);
	x = T.colPivHouseholderQr().solve(b);
	for (int j = 0; j < d + 1; j++) {
		if (x(j) < -ERR) {
			return false;
		}
	}
	return true;
}

Polytope::Polytope() {};

Polytope::Polytope(FILE* input) {};
Polytope::~Polytope() {};

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
	if (ord == P.ord) {
		return true;
	}
	else return false;
	return true;
}; 