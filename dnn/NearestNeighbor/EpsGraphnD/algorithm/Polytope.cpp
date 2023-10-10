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

//Simplices Function
simplex::simplex() {
}
simplex::~simplex() {};
simplex::simplex(int n, vector<Point*> vec) {
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

MatrixXd simplex::getmatrix() {
	return A;
};

bool simplex::intersect(simplex x) {
	MatrixXd B = x.getmatrix();
	MatrixXd T(d + 1, 2 * d + 2);
	// Concatenate
	T.block(0, 0, d, d + 1) = A;
	T.block(0, d + 1, d, d + 1) = -B;
	for (int i = 0; i < 2 * d + 2; i++) {
		if (i < d + 1) T(d, i) = 1;
		else T(d, i) = -1;
	}

	// Find basis of Null space of T
	Eigen::FullPivLU<Eigen::MatrixXd> Temp(T);
	MatrixXd N = Temp.kernel().cast<double>();
	int Row = N.rows();
	int Col = N.cols();

	// Convert eigen structure to alglib data structure
	real_2d_array A;
	double* p = new double[Col * Row];
	for (int i = 0; i < Col * Row; i++) {
		p[i] = N(i / Col, i % Col);
	}

	A.setcontent(Row, Col, p);
	cout << A.tostring(5) << endl;
	double* low = new double[Row];
	for (int i = 0; i < Row; i++) {
		low[i] = ERR;
	}
	real_1d_array al;
	al.setcontent(Row, low);

	double* upp = new double[Row];
	for (int i = 0; i < Row; i++) {
		upp[i] = INFINITY;
	}
	real_1d_array au;
	au.setcontent(Row, upp);

	double* c1 = new double[Col];
	double* c2 = new double[Col];
	for (int i = 0; i < Col; i++) {
		c1[i] = 1;
		c2[i] = 1;
	}
	real_1d_array c;
	real_1d_array s;
	c.setcontent(Col, c2);
	s.setcontent(Col, c1);

	double* v_low = new double[Col];
	for (int i = 0; i < Col; i++) {
		v_low[i] = -INFINITY;
	}
	real_1d_array bndl;
	bndl.setcontent(Col, v_low);

	double* v_upp = new double[Col];
	for (int i = 0; i < Col; i++) {
		v_upp[i] = INFINITY;
	}
	real_1d_array bndu;
	bndu.setcontent(Col, v_upp);

	// Problem Setting
	real_1d_array sol;
	minlpstate state;
	minlpreport rep;
	minlpcreate(Col, state);
	minlpsetcost(state, c);
	minlpsetbc(state, bndl, bndu);
	minlpsetlc2dense(state, A, al, au, Row);
	minlpsetscale(state, s);

	// Solve
	minlpoptimize(state);
	minlpresults(state, sol, rep);
	if (0 < rep.terminationtype && rep.terminationtype < 5) {
		return true;
	}
	return false;
}

bool simplex::intersect(Point* p, Point* q) {
	MatrixXd T(d + 1, d + 3);
	// Concatenate
	T.block(0, 0, d, d + 1) = A;
	vector<double> tmp1 = p->getxs();
	vector<double> tmp2 = q->getxs();
	for (int i = 0; i < d; i++) {
		T(i, d + 1) = -tmp1[i];
		T(i, d + 2) = -tmp2[i];
	}
	for (int i = 0; i < d + 3; i++) {
		if (i < d + 1) T(d, i) = 1;
		else T(d, i) = -1;
	}

	// Find basis of Null space of T
	Eigen::FullPivLU<Eigen::MatrixXd> Temp(T);
	MatrixXd N = Temp.kernel().cast<double>();
	int Row = N.rows();
	int Col = N.cols();

	// Convert eigen structure to alglib data structure
	real_2d_array A;
	double* a = new double[Col * Row];
	for (int i = 0; i < Col * Row; i++) {
		a[i] = N(i / Col, i % Col);
	}

	A.setcontent(Row, Col, a);
	cout << A.tostring(5) << endl;
	double* low = new double[Row];
	for (int i = 0; i < Row; i++) {
		low[i] = ERR;
	}
	real_1d_array al;
	al.setcontent(Row, low);

	double* upp = new double[Row];
	for (int i = 0; i < Row; i++) {
		upp[i] = INFINITY;
	}
	real_1d_array au;
	au.setcontent(Row, upp);

	double* c1 = new double[Col];
	double* c2 = new double[Col];
	for (int i = 0; i < Col; i++) {
		c1[i] = 1;
		c2[i] = 1;
	}
	real_1d_array c;
	real_1d_array s;
	c.setcontent(Col, c2);
	s.setcontent(Col, c1);

	double* v_low = new double[Col];
	for (int i = 0; i < Col; i++) {
		v_low[i] = -INFINITY;
	}
	real_1d_array bndl;
	bndl.setcontent(Col, v_low);

	double* v_upp = new double[Col];
	for (int i = 0; i < Col; i++) {
		v_upp[i] = INFINITY;
	}
	real_1d_array bndu;
	bndu.setcontent(Col, v_upp);

	// Problem Setting
	real_1d_array sol;
	minlpstate state;
	minlpreport rep;
	minlpcreate(Col, state);
	minlpsetcost(state, c);
	minlpsetbc(state, bndl, bndu);
	minlpsetlc2dense(state, A, al, au, Row);
	minlpsetscale(state, s);

	// Solve
	minlpoptimize(state);
	minlpresults(state, sol, rep);
	if (0 < rep.terminationtype && rep.terminationtype < 5) {
		return true;
	}	
	return false;
}

bool simplex::isIn(Point* p) {
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

// Polytope Function
Polytope::Polytope() {};
Polytope::Polytope(FILE* input) {};
Polytope::~Polytope() {};
Polytope::Polytope(vector<simplex> cmp, std::vector<Point*> vt) {
	simplices = cmp;
	vertices = vt;
	num_simplices = cmp.size();
	num_points = vt.size();
	d = vt[0]->getsize();
	for (int i = 0; i++; i < d) {
		xs_min.push_back(INFINITY);
		xs_max.push_back(-INFINITY);
	}
	for (auto vertex : this->vertices) {
		for (int i = 0; i++; i < d) {
			if (xs_min[i] > vertex->getxs()[i]) {
				xs_min[i] = vertex->getxs()[i];
			}
			if (xs_max[i] < vertex->getxs()[i]) {
				xs_max[i] = vertex->getxs()[i];
			}
		}
	}
}
int Polytope::get_num_point() {
	return num_points;
};

std::vector<Point*> Polytope::get_vertices() {
	return vertices;
};

bool Polytope::isIn(Point* p) {
	for (auto simp1 : this->simplices) {
		bool in = simp1.isIn(p);
		if (in) { return true; }
	}
	return false;
};

bool Polytope::intersect(Polytope P) {
	for (auto simp1 : this->simplices) {
		for (auto simp2 : P.simplices) {
			bool intersect = simp1.intersect(simp2);
			if (intersect) { return true; }
		}
	}
	return false;
};

bool Polytope::intersect(Point* p, Point* q) {
	for (auto simp1 : this->simplices) {
		bool intersect = simp1.intersect(p, q);
		if (intersect) { return true; }
	}
	return false;
};

bool Polytope::operator==(Polytope P) {
	if (ord == P.ord) {
		return true;
	}
	else return false;
};

void Polytope::set_vertices(std::vector<Point*> _vertices) {
	this->num_points = _vertices.size();
	this->vertices = _vertices;
}

void Polytope::set_simplices(std::vector<simplex>& _simplices) {
	this->num_simplices = _simplices.size();
	this->simplices = _simplices;
}

