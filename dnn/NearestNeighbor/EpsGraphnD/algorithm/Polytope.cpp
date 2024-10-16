#include "Polytope.h"
#include "optimization.h"
#include <queue>
#include <assert.h>
#include <random>
#include<Eigen/Dense>
#include<Eigen/LU>

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

/*
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
	//cout << A.tostring(5) << endl;
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
	//cout << A.tostring(5) << endl;
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

bool simplex::intersect(Grid_Point* p, Grid_Point* q) {
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
	//cout << A.tostring(5) << endl;
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
*/

bool simplex::intersect(simplex x) {
	for (auto v1 : this->vertices) {
		for (auto v2 : this->vertices) {
			if (v1 != v2 && x.intersect(v1, v2)) { return true; }
		}
	}
	for (auto v1 : x.vertices) {
		for (auto v2 : x.vertices) {
			if (v1 != v2 && this->intersect(v1, v2)) { return true; }
		}
	}
	return false;
}

bool simplex::intersect(Point* p, Point* q) {
	MatrixXd P(d, 1);
	MatrixXd Q(d, 1);

	// One of endpoints is contained in the simplex
	if (this->isIn(P) || this->isIn(Q)) { return true; }

	// Find the intersection of hyperplanes and PQ
	vector<double> lambda;
	for (int j = 0; j < d; j++) {
		P(j, 0) = p->getx(j);
		Q(j, 0) = q->getx(j);
	}
	MatrixXd T(d + 1, d + 1);
	T.block(0, 0, d + 1, d) = A.transpose();
	for (int i = 0; i < d + 1; i++) {
		T(i, d) = 1.0;
	}

	for (int j = 0; j < d + 1; j++) {
		MatrixXd Temp(d, d + 1);
		if (j > 0) {
			Temp.block(0, 0, j, d + 1) = T.block(0, 0, j, d + 1);
		}
		if (j < d + 1) {
			Temp.block(j, 0, d - j, d + 1) = T.block(j + 1, 0, d - j, d + 1);
		}
		// Find basis of Null space of T
		Eigen::FullPivLU<Eigen::MatrixXd> Null(Temp);
		Null.setThreshold(1e-5);
		MatrixXd N = Null.kernel().cast<double>();
		double denom;
		denom = (N.block(0, 0, d, 1).transpose() * (P - Q))(0, 0);
		double numer = 0.0;
		numer = (N.block(0, 0, d, 1).transpose() * Q)(0, 0) + N(d, 0);
		double lambda_cand;
		lambda_cand = -numer / denom;
		if (lambda_cand > 0 && lambda_cand < 1) {
			lambda.push_back(lambda_cand);
		}
	}

	sort(lambda.begin(), lambda.end());

	// Check if the middle point is contained in the simplex
	for (int i = 0; i < lambda.size() - 1; i++) {
		MatrixXd Temp(d, 1);
		Temp = (lambda[i] + lambda[i + 1]) / 2 * P + (1 - (lambda[i] + lambda[i + 1]) / 2) * Q;
		if (this->isIn(Temp)) {
			return true;
		}
	}
	return false;
}

bool simplex::intersect(Grid_Point* p, Grid_Point* q) {
	MatrixXd P(d, 1);
	MatrixXd Q(d, 1);

	// One of endpoints is contained in the simplex
	if (this->isIn(P) || this->isIn(Q)) { return true; }

	// Find the intersection of hyperplanes and PQ
	vector<double> lambda;
	for (int j = 0; j < d; j++) {
		P(j, 0) = p->getx(j);
		Q(j, 0) = q->getx(j);
	}
	MatrixXd T(d + 1, d + 1);
	T.block(0, 0, d + 1, d) = A.transpose();
	for (int i = 0; i < d + 1; i++) {
		T(i, d) = 1.0;
	}

	for (int j = 0; j < d + 1; j++) {
		MatrixXd Temp(d, d + 1);
		if (j > 0) {
			Temp.block(0, 0, j, d + 1) = T.block(0, 0, j, d + 1);
		}
		if (j < d + 1) {
			Temp.block(j, 0, d - j, d + 1) = T.block(j + 1, 0, d - j, d + 1);
		}
		// Find basis of Null space of T
		Eigen::FullPivLU<Eigen::MatrixXd> Null(Temp);
		Null.setThreshold(1e-5);
		MatrixXd N = Null.kernel().cast<double>();
		double denom;
		denom = (N.block(0, 0, d, 1).transpose() * (P - Q))(0, 0);
		double numer = 0.0;
		numer = (N.block(0, 0, d, 1).transpose() * Q)(0, 0) + N(d, 0);
		double lambda_cand;
		lambda_cand = -numer / denom;
		if (lambda_cand > 0 && lambda_cand < 1) {
			lambda.push_back(lambda_cand);
		}
	}

	sort(lambda.begin(), lambda.end());

	// Check if the middle point is contained in the simplex
	for (int i = 0; i < lambda.size() - 1; i++) {
		MatrixXd Temp(d, 1);
		Temp = (lambda[i] + lambda[i + 1]) / 2 * P + (1 - (lambda[i] + lambda[i + 1]) / 2) * Q;
		if (this->isIn(Temp)) {
			return true;
		}
	}
	return false;
}

bool simplex::isIn(Point* p) {
	MatrixXd T(d + 1, d + 1);
	MatrixXd T_temp(d + 1, d + 1);
	MatrixXd X(d, 1);
	vector<double> tmp = p->getxs();
	for (int j = 0; j < d; j++) {
		X(j, 0) = tmp[j];
	}
	for (int i = 0; i < d + 1; i++) {
		T(d, i) = 1;
		T_temp(d, i) = 1;
	}
	T_temp.block(0, 0, d, d + 1) = A.block(0, 0, d, d + 1);
	double T_det = T_temp.determinant();
	for (int j = 0; j < d + 1; j++) {
		if (j > 0) {
			T.block(0, 0, d, j) = A.block(0, 0, d, j);
		}
		if (j < d) {
			T.block(0, j + 1, d, d - j) = A.block(0, j + 1, d, d - j);
		}
		T.block(0, j, d, 1) = X;
		if (T.determinant() * T_det <= 0) {
			return false;
		}
	}
	return true;
}

bool simplex::isIn(MatrixXd X) {
	MatrixXd T(d + 1, d + 1);
	MatrixXd T_temp(d + 1, d + 1);
	for (int i = 0; i < d + 1; i++) {
		T(d, i) = 1;
		T_temp(d, i) = 1;
	}
	T_temp.block(0, 0, d, d + 1) = A.block(0, 0, d, d + 1);
	double T_det = T_temp.determinant();
	for (int j = 0; j < d + 1; j++) {
		if (j > 0) {
			T.block(0, 0, d, j) = A.block(0, 0, d, j);
		}
		if (j < d) {
			T.block(0, j + 1, d, d - j) = A.block(0, j + 1, d, d - j);
		}
		T.block(0, j, d, 1) = X;
		if (T.determinant() * T_det <= 0) {
			return false;
		}
	}
	return true;
}

bool simplex::In(Point* p, Point* q) {
	for (auto vert : vertices) {
		if (p == vert) {
			for (auto vert2 : vertices) {
				if (q == vert2) {
					return true;
				}
			}
		}
	}
	return false;
}

/*
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
}*/

bool simplex::isIn(Point* p) {
	MatrixXd T(d + 1, d + 1);
	MatrixXd T_temp(d + 1, d + 1);
	MatrixXd X(d, 1);
	vector<double> tmp = p->getxs();
	for (int j = 0; j < d; j++) {
		X(j, 0) = tmp[j];
	}
	for (int i = 0; i < d + 1; i++) {
		T(d, i) = 1;
		T_temp(d, i) = 1;
	}
	T_temp.block(0, 0, d, d + 1) = A.block(0, 0, d, d + 1);
	double T_det = T_temp.determinant();
	for (int j = 0; j < d + 1; j++) {
		if (j > 0) {
			T.block(0, 0, d, j) = A.block(0, 0, d, j);
		}
		if (j < d) {
			T.block(0, j + 1, d, d - j) = A.block(0, j + 1, d, d - j);
		}
		T.block(0, j, d, 1) = X;
		if (T.determinant() * T_det < 0) {
			return false;
		}
	}
	return true;
}

// Polytope Function
Polytope::Polytope() {};
Polytope::Polytope(int dimension) {
	this->d = dimension;
};
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

bool Polytope::intersect(Grid_Point* p, Grid_Point* q) {
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
/*
bool Polytope::operator==(const Polytope* P) {
	return ord == P->ord;
}

bool Polytope::operator==(const Polytope& P) {
	return ord == P.ord;
}
*/
//bool operator==(const Polytope t1, const Polytope t2) {
//	return t1.ord == t2.ord;
//}

void Polytope::set_vertices(std::vector<Point*> _vertices) {
	this->num_points = _vertices.size();
	this->vertices = _vertices;
}

void Polytope::set_vertices_size() {
	this->num_points = this->vertices.size();
}

void Polytope::set_simplices(std::vector<simplex>& _simplices) {
	this->num_simplices = _simplices.size();
	this->simplices = _simplices;
}

void Polytope::set_maxmin()
{
	this->d = this->vertices[0]->getsize();
	//cout << "d: " << this->d << endl;
	for (int i = 0; i < this->d; i++) {
		this->xs_max.push_back(DBL_MIN);
		this->xs_min.push_back(DBL_MAX);
	}
	for (int i = 0; i < this->d; i++) {
		for(int j=0;j<this->num_points;j++){
			this->xs_max[i] = (this->xs_max[i] > this->vertices[j]->getx(i)) ? this->xs_max[i] : this->vertices[j]->getx(i);
			this->xs_min[i] = (this->xs_min[i] < this->vertices[j]->getx(i)) ? this->xs_min[i] : this->vertices[j]->getx(i);
		}
	}
	
	//cout << "xs_max: ";
	//for (int i = 0; i < this->d; i++) {
	//	cout << this->xs_max[i] << " ";
	//}

	//cout << endl << "xs_min: ";
	//for (int i = 0; i < this->d; i++) {
	//	cout << this->xs_min[i] << " ";
	//}
	
}

bool Polytope::sameIn(Point *p, Point *q) {
	for (auto simp : simplices) {
		if (simp.In(p, q)) {
			return true;
		}
	}
	return false;
}